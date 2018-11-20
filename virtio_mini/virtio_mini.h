#include <linux/proc_fs.h>
#include <linux/virtio.h>

#ifndef VIRTIO_ID_MINI
#define VIRTIO_ID_MINI 21
#endif

#define VIRTIO_MINI_STRING "virtio-mini"

MODULE_AUTHOR("Matthias Prangl");
MODULE_DESCRIPTION("virtio example front-end driver");
MODULE_LICENSE("GPL v2");

static struct virtio_device_id id_table[] = {
	{ VIRTIO_ID_MINI, VIRTIO_DEV_ANY_ID },
	{ 0 },
};

static unsigned int feature_table[] = { };

struct virtio_mini_device {
    /* receiving virtqueue */
    struct virtqueue *vq_rx;
    /* transmitting virtqueue */
    struct virtqueue *vq_tx;
    /* related virtio_device */
    struct virtio_device *vdev;
    /* proc dir entry for this instance of the device */
    struct proc_dir_entry *pde;
    /* store length of last sent message */
    unsigned int prev_len;
};

static int virtio_mini_open(struct inode *inode, struct  file *file);

void virtio_mini_vq_tx_cb(struct virtqueue *vq);
void virtio_mini_vq_rx_cb(struct virtqueue *vq);

int virtio_mini_assign_virtqueue(struct virtio_mini_device *vmini);
int probe_virtio_mini(struct virtio_device *vdev);

void remove_virtio_mini (struct virtio_device *vdev);

static struct file_operations pde_fops = {
    .owner = THIS_MODULE,
    .open = virtio_mini_open,
};

static struct virtio_driver driver_virtio_mini = {
    .driver.name = KBUILD_MODNAME,
    .driver.owner = THIS_MODULE,
    .id_table = id_table,
    .feature_table = feature_table,
    .feature_table_size = ARRAY_SIZE(feature_table),
    .probe = probe_virtio_mini,
    .remove = remove_virtio_mini
};