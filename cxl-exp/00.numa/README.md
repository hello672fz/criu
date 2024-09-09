00.numa
---

## Install dependencies

````shell
sudo apt install libnuma-dev
````

## Part 1: verify the effectiveness of NUMA memory allocation methods

> Annotation: Evaluation at node-126 (IP address: 192.168.1.126).
> Source codes can be found at the path `/home/lgw/cxl-exp`.

### To eliminate the effect of page cache

```shell
root@tank:~# echo 3 > /proc/sys/vm/drop_caches
```

### How to observe the behavior of memory allocation?

Continuously monitor memory allocation for each NUMA node by executing the following command:

```shell
root@tank:~# watch numactl -H
Every 2.0s: numactl -H            tank: Sat Apr 20 15:52:12 2024

available: 4 nodes (0-3)
node 0 cpus: 0 1 2 3 4 5 6 7 8 9 40 41 42 43 44 45 46 47 48 49
node 0 size: 64383 MB
node 0 free: 62720 MB
node 1 cpus: 10 11 12 13 14 15 16 17 18 19 50 51 52 53 54 55 56 57 58 59
node 1 size: 64507 MB
node 1 free: 62866 MB
node 2 cpus: 20 21 22 23 24 25 26 27 28 29 60 61 62 63 64 65 66 67 68 69
node 2 size: 64507 MB
node 2 free: 62570 MB
node 3 cpus: 30 31 32 33 34 35 36 37 38 39 70 71 72 73 74 75 76 77 78 79
node 3 size: 64465 MB
node 3 free: 63527 MB
node distances:
node   0   1   2   3
  0:  10  21  21  21
  1:  21  10  21  21
  2:  21  21  10  21
  3:  21  21  21  10
```

### NUMA memory allocation methods

There are two methods to allocate memory from a specified NUMA node:
- One involves using the API `numa_alloc_onnode()` within the `numa.h` library.
  The source code can be accessed at `alloc_memory_with_numa.c`.
- Another utilizes the general API `malloc()`, but employs the `numactl` command to execute memory binding when runing the process.
  The source code can be accessed at `alloc_memory_wo_numa.c`.


```shell
# step1: build 00.numa
cd cxl-exp/
mkdir /tmp/cxl-exp2
cmake -B /tmp/cxl-exp2
cmake --build /tmp/cxl-exp2 --target 00_alloc_memory_wo_numa 00_alloc_memory_with_numa

# step2: verify method 1. After running the process, 
# you can easily observe the decrease in free memory in node 1.
/tmp/cxl-exp2/00_alloc_memory_with_numa

# step2: verify method 2. After running the process, 
# you can easily observe the decrease in free memory in node 1.
numactl --membind 1 /tmp/cxl-exp/00_alloc_memory_wo_numa
```

**Conclusion:**
We have observed that both methods can be used to allocate memory from a specified NUMA node.
However, there are some considerations to keep in mind.
When allocating memory using either malloc or numa_alloc_onnode,
the OS just reserves virtual memory for your process.
In fact, physical memory is allocated when your process actually accesses the memory (like writing some data).


## Part 2: Bringing these methods to OpenFaaS

> Annotation: Evaluation at node-137 (IP address: 192.168.1.137).
> Source codes can be found at the path `/home/tank/openfaas`.

### Download OpenFaaS templates

````shell
faas-cli template pull
````

### Create a new NUMA-aware template

```shell
# step1: choose python3-debian as a base template
cp -r template/python3-debian template/python3-debian-numa

# step2: modify template's Dockerfile. There are two parts need to be modified.
# More details can be found in the path `openfaas/template/python3-debian-numa/Dockerfile`.
vim template/python3-debian-numa/Dockerfile
```


### Create a new Function using the NUMA-aware template

```shell
faas-cli new --lang python3-debian-numa test-numa-function
```

### Customize handler function
```shell
# Implement a Python program 
# that allocates 2MB of memory per iteration and runs this operation 1000 times.
# More details can be found in the path `openfaas/test-numa_function`.
vim test-numa-function/handler.py
```

### Build Docker image

```shell
# Modify the 'image' field by adding the DockerHub account name. 
# More details can be found in the path `openfaas/test-numa-function.yml`.
vim test-numa-function.yml

# Build image
faas-cli build -f test-numa-function.yml
```

### Verify the image independently, with involving OpenFaaS

```shell
# step 1: clear cache
echo 3 > /proc/sys/vm/drop_caches

# step 2: Launch a monitor on another terminal
watch numactl -H

# step 3: run docker
docker run --privileged -p 10020:8080 jxb2018/test-numa-function

# step 4: invoke
curl -X POST http://localhost:10020
```

Afterward, we will monitor the reduction in free memory within NUMA node 1.
However, we have also observed changes within NUMA node 0.
This unexpected behavior puzzles us.
We suspect that Python may be the cause.
To validate this suspicion, we require an OpenFaaS Function implemented in the C programming language.


### Deploy to OpenFaaS

```shell
# step 1: push image 
docker push jxb2018/test-numa-function:latest

# step 2: deploy
faas-cli deploy -f test-numa-function.yml
```

### Give containers "privileged" mode

```shell
# step 1: patch the securityContext
kubectl patch deployment test-numa-function \
  -n openfaas-fn \
  --patch '{"spec": {
              "template": {
                "spec": {
                  "containers": [
                    {
                      "env": [{"name": "fprocess", "value": "numactl --membind 1 python3 index.py"}],
                      "name": "test-numa-function",
                      "image": "jxb2018/test-numa-function:latest", 
                      "securityContext": 
                        {"privileged": true}
                    }
                  ]
                }
              }
           }}'

# step 2: invoke
curl -X POST http://192.168.1.137:31112/function/test-numa-function
```




