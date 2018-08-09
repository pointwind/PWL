#pragma once
#include <cstdio>
#include <deque>
namespace PWL
{
	template<typename T>class Node
	{
	public:
		bool color = false;//false == red
		Node<T> * par;        // to the parent
		Node<T> * left;			//to the left side son
		Node<T> * right;        //to the right side son

								/*****************************************************/
		T number;          // the unique number

	};
	template<typename T> class RB_tree
	{
	public:
		RB_tree()
		{
			root = &rooting;
			root->color = 1;
			nil = &nilling;
			nil->color = 1;
			root->par = &nilling;
			root->left = nil;
			root->right = nil;
		}
		void insert_node(Node<T>* student);//增加节点
		void delete_node(Node<T>* students);//删除节点

		Node<T>* search(Node<T>* student)const;//按key搜索某个固定的节点，并返回他

		void left_rotate(Node<T>* student);//左旋
		void right_rotate(Node<T>* student);//右旋


		void display(Node<T>* root)const;  //前序遍历
		void or_display(Node<T>* root)const;//中序遍历
		void la_display(Node<T>* root)const;//后序遍历
		void layer_display(Node<T>* root);//层序遍历


		void insert_node_fix(Node<T>* student);//增加节点修正
		void delete_node_fix(Node<T>* student);//删除节点修正
		void instead(Node<T>* lhs, Node<T>* rhs);//lhs will been deleted,and the lhs' parent will point to the rhs
		Node<T>* root;  //the root node
		Node<T> rooting;//
		Node<T>* nil;  //the nil node
		Node<T> nilling;//
		int count = 0;//代表是否为第一次创建
	};

	template<typename T>void RB_tree<T>::left_rotate(Node<T>* student)//左旋
	{
		Node<T>* y = student->right;
		student->right = y->left;
		y->left->par = student;
		y->par = student->par;
		if (student->par == nil)
		{
			root = y;
		}
		else
		{
			if (student == student->par->left)    student->par->left = y;
			else                                 student->par->right = y;
		}
		y->left = student;
		student->par = y;
	}
	template<typename T>void RB_tree<T>::right_rotate(Node<T>* student)//右旋
	{
		Node<T>* y = student->left;
		student->left = y->right;
		y->right->par = student;
		y->par = student->par;
		if (student->par == nil)
		{
			root = y;
		}
		else
		{
			if (student == student->par->left)    student->par->left = y;
			else                                 student->par->right = y;
		}
		y->right = student;
		student->par = y;

	}

	template<typename T>void RB_tree<T>::insert_node(Node<T>* student)//增加节点
	{
		student->left = nil;
		student->right = nil;
		Node<T>* x = root;
		Node<T>*y = nullptr;

		while (x != nil)
		{
			if (count == 0)
			{
				break;
			}
			y = x;
			if (student->number < x->number)	x = x->left;
			else x = x->right;
		}
		if (count != 0)
		{
			student->par = y;//make y to be it's parent
		}

		if (y != nullptr)//make y to point to the node
		{
			if (student->number < y->number)	y->left = student;
			else    y->right = student;
		}
		else
		{
			root->number = student->number;
		}
		if (count != 0)
		{
			insert_node_fix(student);
		}
		count = 1;
	}
	template<typename T>void RB_tree<T>::insert_node_fix(Node<T>* student)//增加节点修正
	{
		while (student->par->color == false && student->par != nil)
		{
			if (student->par == student->par->par->left)
			{
				Node<T>*uncle = student->par->par->right;
				if (uncle->color == false)
				{
					student->par->color = true;
					student->par->par->color = false;
					uncle->color = true;
					student = student->par->par;
				}
				else if (student == student->par->right)
				{
					student = student->par;
					left_rotate(student);
				}
				student->par->color = true;
				student->par->par->color = false;
				right_rotate(student->par->par);
			}
			else
			{
				Node<T>*uncle = student->par->par->left;
				if (uncle->color == false)
				{
					student->par->color = true;
					student->par->par->color = false;
					uncle->color = true;
					student = student->par->par;
					continue;
				}
				else if (student == student->par->left)
				{
					student = student->par;
					right_rotate(student);
				}
				student->par->color = true;
				student->par->par->color = false;
				left_rotate(student->par->par);
			}
		}
		root->color = true;
	}

	template<typename T>void RB_tree<T>::instead(Node<T>* lhs, Node<T>* rhs)//lhs will been deleted,and the lhs' parent will point to the rhs
	{
		if (lhs->par == nil)
		{
			root = rhs;
		}
		else if (lhs == lhs->par->left)
		{
			lhs->par->left = rhs;
		}
		else if (lhs == lhs->par->right)
		{
			lhs->par->right = rhs;
		}
	}
	template<typename T>void RB_tree<T>::delete_node(Node<T>* students)//delete a node
	{
		Node<T>* student = search(students);
		Node<T>* instead_node;//this node  will move to the the node which will been deleted

		Node<T>* next = student;//y
		bool next_origin_color = next->color;

		if (student->left == nil)
		{
			instead_node = student->right;
			instead(student, student->right);
		}
		else if (student->right == nil)
		{
			instead_node = student->left;
			instead(student, student->left);
		}
		else// both of the left and right isn't empty
		{
			next = student->right;
			while (next->left != nil)
			{
				next = next->left;
			}


			next_origin_color = next->color;
			instead_node = next->right;
			if (next->par == student)//the student's right 's left is empty
			{
				instead_node->par = next;
			}
			else//make next's right to get it's sit
			{
				instead(next, next->right);
				next->right = student->right;
				next->right->par = next;
			}
			instead(student, next);
			next->left = student->left;
			next->left->par = next;
			next->color = student->color;
		}
		if (next_origin_color == true)
		{
			delete_node_fix(instead_node);
		}
	}
	template<typename T>void RB_tree<T>::delete_node_fix(Node<T>* student)//this student is the instead_node
	{
		Node<T>* brother;
		while (student != root && student->color == true && student != nil)
		{
			if (student == student->par->left)
			{
				brother = student->par->right;
				if (brother == nil) break;
				if (brother->color == false)//case 1
				{
					brother->color = true;
					student->par->color = false;
					left_rotate(student->par);
					brother = student->par->right;
				}
				if (brother->left->color == true && brother->right->color == true)//ccase 2
				{
					brother->color = false;
					student = student->par;
				}
				else if (brother->right->color == true)//case3
				{
					brother->left->color = true;
					brother->color = false;
					right_rotate(brother);
					brother = student->par->right;
				}
				brother->color = student->par->color;
				student->par->color = true;
				brother->right->color = true;
				left_rotate(student->par);
				student = root;
			}
			else
			{
				brother = student->par->left;
				if (brother == nil) break;
				if (brother->color == false)//case 1
				{
					brother->color = true;
					student->par->color = false;
					right_rotate(student->par);
					brother = student->par->left;
				}
				if (brother->right->color == true && brother->left->color == true)//ccase 2
				{
					brother->color = false;
					student = student->par;
				}
				else if (brother->left->color == true)//case3
				{
					brother->right->color = true;
					brother->color = false;
					left_rotate(brother);
					brother = student->par->left;
				}
				brother->color = student->par->color;
				student->par->color = true;
				brother->left->color = true;
				right_rotate(student->par);
				student = root;
			}
		}

	}

	template<typename T>void RB_tree<T>::display(Node<T>* root)const//pre-print all of the nodes
	{
		if (root == nil) return;
		printf("%d \n", root->number);
		display(root->left);
		display(root->right);
	}
	template<typename T>void RB_tree<T>::or_display(Node<T>* root)const//or-print all of the nodes
	{
		if (root == nil) return;
		display(root->left);
		printf("%d \n", root->number);
		display(root->right);
	}
	template<typename T>void RB_tree<T>::la_display(Node<T>* root)const//la-print all of the nodes
	{
		if (root == nil) return;
		display(root->left);
		display(root->right);
		printf("%d \n", root->number);
	}
	template<typename T>void RB_tree<T>::layer_display(Node<T>* root)//层序遍历
	{
		std::deque<Node<T>*> que;
		que.push_back(root);
		while (!que.empty())
		{
			Node<T> * front = que.front();
			if (front == nil) break;
			printf("%d \n", front->number);
			que.pop_front();
			if (front->left != nil)
			{
				que.push_back(front->left);
			}
			if (front->right != nil)
			{
				que.push_back(front->right);
			}
		}
	}





	template<typename T>Node<T>* RB_tree<T>::search(Node<T>* student)const//搜索指点key的节点
	{
		Node<T>* ptr = root;
		while (ptr != nil) {
			if (student->number < ptr->number)
			{
				ptr = ptr->left;
			}
			if (student->number > ptr->number)
			{
				ptr = ptr->right;
			}
			if (student->number == ptr->number)
			{
				return ptr;
			}
		}
		return nil;
	}
}