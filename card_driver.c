////////////////////////////////////////////////////////////////////////
////////////////// CS449 PROJECT 4 - PETER STAMOS //////////////////////
////////////////////////////////////////////////////////////////////////

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>

#include <asm/uaccess.h>

static char cards[52];

static char i;

////////////////////////////////////////////////////////////////////////

void initializer(void) {

  int j = 0;

  for (j = 0; j < 52; j++) {
    cards[j] = j + 1;
  }
}

////////////////////////////////////////////////////////////////////////

unsigned char get_random_byte(int max) {

  unsigned char c;

  get_random_bytes(&c, 1);

  c = (c < 0)? 0-c: c;

  return c%max;
}

////////////////////////////////////////////////////////////////////////

void shuffle(int x) {

  while (x > 0) {

    char random;
    int k;

    random = get_random_byte(52);

    for (k = 0; k < 52; k++) {

      int y;
      char z;

      y = (k + random) % 52;

      z = cards[k];

      cards[k] = cards[y];
      cards[y] = z;
    }

    x--;
  }
}

////////////////////////////////////////////////////////////////////////

char random_card(void) {

  if (i >= 51) {
    shuffle(20);
    i = 0;
  }

  return cards[i++];
}

////////////////////////////////////////////////////////////////////////

static ssize_t card_read(struct file * file, char * buf, size_t count, loff_t *ppos) {

  char c;
  int l;

  for(l=0; l < count; l++){

    c = random_card();
    copy_to_user(buf, &c, 1);

    buf++;
    *ppos = *ppos + 1;
  }

  return count;
}

////////////////////////////////////////////////////////////////////////

static const struct file_operations card_fops = {
	.owner		= THIS_MODULE,
	.read		= card_read,
};

////////////////////////////////////////////////////////////////////////

static struct miscdevice card_dev = {

  MISC_DYNAMIC_MINOR,
  "cards",
  &card_fops
};

////////////////////////////////////////////////////////////////////////

static int __init card_init(void) {

	int ret;
	i = 0;

  initializer();
  shuffle(40);

  ret = misc_register(&card_dev);

  if (ret) {
		printk(KERN_ERR "Unable to register \"Card_Driver\" misc device\n");
  }

	return ret;
}

////////////////////////////////////////////////////////////////////////

module_init(card_init);

static void __exit

card_exit(void) {
  misc_deregister(&card_dev);
}

module_exit(card_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("dev");
MODULE_AUTHOR("Peter Stamos");

////////////////////////////////////////////////////////////////////////
////////////////// CS449 PROJECT 4 - PETER STAMOS //////////////////////
////////////////////////////////////////////////////////////////////////
