#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

/*
	FILE : linked_list.h

	链表宏定义头文件,为了方便链表的使用
*/

/*
数据插入到链表
*/
#define INSERT_TO_LIST( list, item, prev, next )
if ( !list )
{
	list = item;
	list->prev = list->next = list;
}
else
{
	item->prev = list->prev;
	item->next = list;
	list->prev = item;
	item->prev->next = item;
}

/*
从链表中取数据
*/
#define REMOVE_FROM_LIST( list, item, prev, next )
if ( item == list )
{
	if (item == item->next)
		list = NULL;
	else
		list = item->next;
}
if (list)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;
}

/*
链表数据检索循环 -开始- 的宏定义
*/
#define LIST_WHILE( list, item, temp_d, next )
if ((item = list))
{
	do{
		temp_d = item->next;


/*
链表数据检索循环 -结束- 的宏定义
*/
#define LIST_WHILEEND( list, item, temp_d )
		item = temp_d;
		}while( list && item && (item != list) );
}

/*
跳过链表的宏定义
*/
#define LIST_SKIP( item, temp_d )
{
	item = temp_d;
	continue;
}

#endif	// LINKED_LIST_H_

// EOF