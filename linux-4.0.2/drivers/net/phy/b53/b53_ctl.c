/*
 * b53_ctl.c
 *
 *  Created on: Dec 25, 2017
 *      Author: zhurish
 */

/*
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/capability.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <asm/mach-types.h>
#include <asm/uaccess.h>
*/
#include <linux/kernel.h>
#include <linux/phy.h>
#include <linux/module.h>
#include "b53_priv.h"
#include "b53_regs.h"

/****************************************************************************************/
//#define B53_CTL_DEBUG
#define DEVICE_NAME	"bcm53125"
#define MODULE_NAME "bcm53"

static struct b53_device *b53_device_ctl = NULL;
/****************************************************************************************/
/****************************************************************************************/
static ssize_t b53_ctl_read(struct file *file, char __user *data, size_t count, loff_t *f_pos);
static ssize_t b53_ctl_write(struct file *file, const char __user *data, size_t count, loff_t *f_pos);
static int b53_ctl_open(struct inode *inode, struct file *file);
static int b53_ctl_release(struct inode *inode, struct file *file);
/*static int b53_ctl_ioctl(struct inode *inode, struct file *file,
		unsigned int cmd, unsigned long arg);*/
/****************************************************************************************/
/****************************************************************************************/
static ssize_t b53_ctl_read(struct file *file, char __user *data, size_t count, loff_t *f_pos)
{
	int ret = 0;
#if 1//def B53_CTL_DEBUG
	struct b53_ctl_data_t ctl_data;
	struct b53_device *dev = file->private_data;

	if(!dev)
	{//EFAULT
		printk(KERN_ERR " read dev is NULL\n");
		return -EFAULT;
	}
/*	if(!dev->phydev)
	{
		printk(KERN_ERR " read dphyev is NULL\n");
		return -EFAULT;
	}*/
	if(!dev->priv)
	{
		printk(KERN_ERR " read mdio bus dev is NULL\n");
		return -EFAULT;
	}
	memset(&ctl_data, 0, sizeof(ctl_data));
	if (copy_from_user(&ctl_data, data, sizeof(struct b53_ctl_data_t)))
		return -EFAULT;
/*	memcpy(&ctl_data, &dev->data, sizeof(struct b53_ctl_data_t));
	if(ctl_data.cmd == B53BIT_WRITE)
	{
		//memcpy(&dev->data, &ctl_data, sizeof(struct b53_ctl_data_t));
		return -EFAULT;
	}*/
/*	ctl_data.bit = B53BIT(32);
	ctl_data.page = 0x02;
	ctl_data.reg = 0x30;*/
	//copy_from_user(&ctl_data, data, sizeof(struct b53_ctl_data_t));
	switch(B53BIT(ctl_data.bit))
	{
	case B53BIT(8):
		ret = b53_read8(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val8);
//		printk(KERN_ERR " read mdio bus dev %x\n",ctl_data.data.val8);
		break;
	case B53BIT(16):
		ret = b53_read16(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val16);
//		printk(KERN_ERR " read mdio bus dev %x\n",ctl_data.data.val16);
		break;
	case B53BIT(32):
		ret = b53_read32(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val32);
//		printk(KERN_ERR " read mdio bus dev %x\n",ctl_data.data.val32);
		break;
	case B53BIT(48):
		ret = b53_read48(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val64);
//		printk(KERN_ERR " read mdio bus dev %x\n",ctl_data.data.val64);
		break;
	case B53BIT(64):
		ret = b53_read64(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val64);
//		printk(KERN_ERR " read mdio bus dev %x\n",ctl_data.data.val64);
		break;
	default:
		printk(KERN_ERR " read mdio bus EOPNOTSUPP\n");
		ret = -EOPNOTSUPP;
		break;
	}
	if(ret == 0)
	{
		if (copy_to_user(data, &ctl_data, sizeof(struct b53_ctl_data_t)))
		{
			printk(KERN_ERR " read mdio bus copy_to_user\n");
			return -EFAULT;
		}
		return 0;
	}
	if(ret == 0)
		return 0;//sizeof(struct b53_ctl_data_t);
	printk(KERN_ERR " read mdio bus return=%d\n",ret);
#endif
	return ret;
}
/****************************************************************************************/
static ssize_t b53_ctl_write(struct file *file, const char __user *data, size_t count, loff_t *f_pos)
{
	int ret = 0;
#if 1//def B53_CTL_DEBUG
	struct b53_ctl_data_t ctl_data;
	struct b53_device *dev = file->private_data;

	if(!dev)
	{
		printk(KERN_ERR " write dev is NULL\n");
		return -EFAULT;
	}
/*	if(!dev->phydev)
	{
		printk(KERN_ERR " write dphyev is NULL\n");
		return -EFAULT;
	}*/
	if(!dev->priv)
	{
		printk(KERN_ERR " write mdio bus dev is NULL\n");
		return -EFAULT;
	}
	//memset(&dev->data, 0, sizeof(struct b53_ctl_data_t));
	if (copy_from_user(&ctl_data, data, sizeof(struct b53_ctl_data_t)))
		return -EFAULT;
	if(ctl_data.cmd == B53BIT_READ)
	{
		//memcpy(&dev->data, &ctl_data, sizeof(struct b53_ctl_data_t));
		return -EFAULT;
	}
	switch(B53BIT(ctl_data.bit))
	{
	case B53BIT(8):
		ret = b53_write8(dev, ctl_data.page, ctl_data.reg, ctl_data.data.val8);
		break;
	case B53BIT(16):
		ret = b53_write16(dev, ctl_data.page, ctl_data.reg, ctl_data.data.val16);
		break;
	case B53BIT(32):
		ret = b53_write32(dev, ctl_data.page, ctl_data.reg, ctl_data.data.val32);
		break;
	case B53BIT(48):
		ret = b53_write48(dev, ctl_data.page, ctl_data.reg, ctl_data.data.val64);
		break;
	case B53BIT(64):
		ret = b53_write64(dev, ctl_data.page, ctl_data.reg, ctl_data.data.val64);
		break;
	default:
		printk(KERN_ERR " read mdio bus EOPNOTSUPP\n");
		ret = -EOPNOTSUPP;
		break;
	}
	if(ret == 0)
		return 0; //sizeof(struct b53_ctl_data_t);
#endif
	return ret;
}
/****************************************************************************************/
static int b53_ctl_open(struct inode *inode, struct file *file)
{
#if 1//def B53_CTL_DEBUG
	file->private_data = b53_device_ctl;
#endif
	return nonseekable_open(inode, file);
}
/****************************************************************************************/
static int b53_ctl_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}
#ifdef B53_CTL_DEBUG
#define BCMCONTROL      1//_IOWR('b', 0x8e, struct b53_ctl_data_t)
/****************************************************************************************/
static int b53_ctl_ioctl(struct inode *inode, struct file *file,
		unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	unsigned int minor = iminor(file->f_path.dentry->d_inode);
	struct b53_ctl_data_t ctl_data;
	struct b53_device *dev = file->private_data;
	 //void __user *argp = (void __user *)arg;

	if (!dev) { //EFAULT
		printk(KERN_ERR " ioctl dev is NULL\n");
		return -EFAULT;
	}
	if (!dev->phydev) {
		printk(KERN_ERR " ioctl dphyev is NULL\n");
		return -EFAULT;
	}
	if (!dev->priv) {
		printk(KERN_ERR " ioctl mdio bus dev is NULL\n");
		return -EFAULT;
	}
	memset(&ctl_data, 0, sizeof(struct b53_ctl_data_t));
	copy_from_user(&ctl_data, (struct b53_ctl_data_t*) arg,
		sizeof(struct b53_ctl_data_t));
	/* First handle the cases that don't take arguments. */
	switch (cmd)
	{
	case BCMCONTROL:
	 //copy_from_user(&ctl_data, data, sizeof(struct b53_ctl_data_t));
		switch(B53BIT(ctl_data.bit))
		{
		case B53BIT(8):
			ret = b53_read8(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val8);
//			printk(KERN_ERR " ioctl read mdio bus dev %x\n",ctl_data.data.val8);
			break;
		case B53BIT(16):
			ret = b53_read16(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val16);
//			printk(KERN_ERR " ioctl read mdio bus dev %x\n",ctl_data.data.val16);
			break;
		case B53BIT(32):
			ret = b53_read32(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val32);
//			printk(KERN_ERR " ioctl read mdio bus dev %x\n",ctl_data.data.val32);
			break;
		case B53BIT(48):
			ret = b53_read48(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val64);
//			printk(KERN_ERR " ioctl read mdio bus dev %x\n",ctl_data.data.val64);
			break;
		case B53BIT(64):
			ret = b53_read64(dev, ctl_data.page, ctl_data.reg, &ctl_data.data.val64);
//			printk(KERN_ERR " ioctl read mdio bus dev %x\n",ctl_data.data.val64);
			break;
		default:
			printk(KERN_ERR " ioctl mdio bus EOPNOTSUPP\n");
			ret = -EOPNOTSUPP;
			break;
		}
		if (ret == 0)
		{
			if (copy_to_user((struct b53_ctl_data_t*) arg, &ctl_data,
				sizeof(struct b53_ctl_data_t)))
			{
				printk(KERN_ERR " ioctl read mdio bus copy_to_user\n");
				return -EFAULT;
			}
		}
		if (ret == 0)
			ret = 0; //sizeof(struct b53_ctl_data_t);
		else
		{
			printk(KERN_ERR " ioctl read mdio bus return=%d\n",ret);
			ret = -EFAULT;
		}
		break;

	default:
		printk(KERN_ERR "%x: What? (cmd=0x%x)\n", minor, cmd);
		ret = -EINVAL;
	}
	/* Keep the compiler happy */
	return ret;
}
#endif
/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/
static const struct file_operations b53_fops =
{
	.owner = THIS_MODULE,
	//.unlocked_ioctl = b53_ctl_ioctl,
	.read = b53_ctl_read,
	.write = b53_ctl_write,
	.open = b53_ctl_open,
	.release = b53_ctl_release,
	.llseek = no_llseek,
};

/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/
int bcm53_ctl_init(struct b53_device *dev)
{
#if 1//def 1//B53_CTL_DEBUG
	//mutex_init(&dev->reg_mutex);
	dev->dev_num = register_chrdev(0, DEVICE_NAME, &b53_fops);
	if (dev->dev_num < 0)
	{
		//mutex_destroy(&dev->reg_mutex);
		printk(KERN_ERR "%s[%d] register_chrdev() error.\n", __FILE__, __LINE__);
		return -1;
	}
	//dev->dev_num = ret;
	if ((dev->bcm53xx_class = class_create(THIS_MODULE, MODULE_NAME)) == NULL)
	{
		//mutex_destroy(&dev->reg_mutex);
		unregister_chrdev(dev->dev_num, DEVICE_NAME);
		printk(KERN_ERR "%s[%d] class_create() error.\n", __FILE__, __LINE__);
		return -1;
	}
	if (device_create(dev->bcm53xx_class, NULL, MKDEV(dev->dev_num, 0), NULL,
			"%s%d", DEVICE_NAME, 0) == NULL)
	{
		//mutex_destroy(&dev->reg_mutex);
		unregister_chrdev(dev->dev_num, DEVICE_NAME);
		printk(KERN_ERR "%s[%d] device_create() error.\n", __FILE__, __LINE__);
		return -1;
	}
	b53_device_ctl = dev;
#endif
	pr_info("bcm53xx ctl driver module load(%d)\n", dev->dev_num);
	return 0;
}
/****************************************************************************************/
/****************************************************************************************/
int bcm53_ctl_exit(struct b53_device *dev)
{
//#ifdef B53_CTL_DEBUG
	if (dev->dev_num != 0)
	{
		unregister_chrdev(dev->dev_num, DEVICE_NAME);
		device_destroy(dev->bcm53xx_class, MKDEV(dev->dev_num, 0));
	}
//	mutex_destroy(&dev->reg_mutex);
	class_destroy(dev->bcm53xx_class);
	dev->bcm53xx_class = NULL;
//#endif
	b53_device_ctl = NULL;
	pr_info("bcm53xx ctl driver module unload\n");
	return 0;
}

#ifndef B53_CTL_DEBUG
EXPORT_SYMBOL(bcm53_ctl_init);
EXPORT_SYMBOL(bcm53_ctl_exit);
#else
int __init b53_ctl_init(void)
{
	int ret;
	ret = bcm53_ctl_init(&b53_device);
	if (ret)
		return ret;
	ret = b53_phy_fixup_register();
	if (ret)
	{
		bcm53_ctl_exit(&b53_device);
		return ret;
	}
	ret = b53_phy_driver_register();
	if (ret)
	{
		bcm53_ctl_exit(&b53_device);
		return ret;
	}
	return ret;
}

void __exit b53_ctl_exit(void)
{
	//b53_phy_driver_unregister();
	b53_phy_driver_unregister();
	bcm53_ctl_exit(&b53_device);
}

module_init( b53_ctl_init);
module_exit( b53_ctl_exit);
#endif
MODULE_DESCRIPTION("B53 MDIO access driver");
MODULE_LICENSE("Dual BSD/GPL");

