//
// Created by lfz on 9/9/24.
//

#include "script.h"
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/numa.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

// 用于保存原始 mmap 函数指针
static int (*orig_mmap)(struct file *, struct vm_area_struct *);

// 我们的自定义 mmap 函数
static int numa_mmap(struct file *filp, struct vm_area_struct *vma) {
	int node = 0; // 指定 NUMA 节点
	int ret;

	// 打印调试信息
	pr_info("numa_mmap: trying to allocate memory from node %d\n", node);

	// 调用内核函数从指定 NUMA 节点分配内存
	ret = numa_mmap_pfns(&init_mm, vma->vm_start, vma->vm_end - vma->vm_start,
			     vma->vm_page_prot, node);

	if (ret) {
		pr_err("numa_mmap: failed to allocate memory from node %d\n", node);
		return -ENOMEM;
	}

	return 0;
}

// 模块初始化函数
static int __init numa_mmap_init(void) {
	// 保存原始 mmap 函数指针
	orig_mmap = (int (*)(struct file *, struct vm_area_struct *))syscalls->sys_mmap;
	// 设置我们的自定义 mmap 函数
	syscalls->sys_mmap = (sys_call_ptr_t)numa_mmap;
	pr_info("numa_mmap: module loaded\n");
	return 0;
}

// 模块卸载函数
static void __exit numa_mmap_exit(void) {
	// 恢复原始 mmap 函数
	syscalls->sys_mmap = (sys_call_ptr_t)orig_mmap;
	pr_info("numa_mmap: module unloaded\n");
}

module_init(numa_mmap_init);
module_exit(numa_mmap_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FZ L");
MODULE_DESCRIPTION("A custom mmap hook for the Linux kernel");
MODULE_VERSION("0.1");