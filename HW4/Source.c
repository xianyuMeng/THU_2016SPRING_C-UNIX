struct list_head {
	struct list_head* parent;
	struct list_head* left;
	struct list_head* right;
};

struct my_node {
	char data[30];
	struct list_head list;
};


