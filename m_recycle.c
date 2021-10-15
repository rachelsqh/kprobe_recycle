#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/binfmts.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include "./hooks.h"
#define BAK_DIR	"/var/bak_dir/"
#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "do_unlinkat";
module_param_string(symbol, symbol, sizeof(symbol), 0644);

static struct kprobe kp = {
	.symbol_name	= symbol,
};


/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	char name_comm[16];
/*	char path_r[256]; */
/*	char *ret_path = NULL; */
	struct filename *name;
/*	struct path path; */
	int ret = 0;

	char new_name[256];
	char old_name[256];
#if 0
	get_fs_pwd(current->fs,&path);
	ret_path = d_path(&path,path_r,256);//pwd
	path_put(&path);

	if(ret_path){
		printk("pwd = %s\n",ret_path);
	}
#endif
	get_task_comm(name_comm,current);

	if(!strcmp(name_comm,"rm")){//rm cmd
		name = (struct filename *)(regs->si);
		if(!name)
			return 0;

		pr_info("task comm:%s filename name = %s\n",name_comm,name->name);//old_name
		
		if(!(name->name))
			return 0;

		sprintf(new_name,"%s%s",BAK_DIR,name->name);
		sprintf(old_name,"%s",name->name);
		ret = bak_file(old_name,new_name);
		pr_info("%s:%d ret = %d\n",__func__,__LINE__,ret);	
	}

	return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
}

static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	return 0;
}
NOKPROBE_SYMBOL(handler_fault);

static int __init m_recycle_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	pr_info("%s:%d\n",__func__,__LINE__);
	return 0;
}

static void __exit m_recycle_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(m_recycle_init)
module_exit(m_recycle_exit)
MODULE_LICENSE("GPL");
