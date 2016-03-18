#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

/*
	FILE : linked_list.h

	����궨��ͷ�ļ�,Ϊ�˷��������ʹ��
*/

/*
���ݲ��뵽����
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
��������ȡ����
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
�������ݼ���ѭ�� -��ʼ- �ĺ궨��
*/
#define LIST_WHILE( list, item, temp_d, next )
if ((item = list))
{
	do{
		temp_d = item->next;


/*
�������ݼ���ѭ�� -����- �ĺ궨��
*/
#define LIST_WHILEEND( list, item, temp_d )
		item = temp_d;
		}while( list && item && (item != list) );
}

/*
��������ĺ궨��
*/
#define LIST_SKIP( item, temp_d )
{
	item = temp_d;
	continue;
}

#endif	// LINKED_LIST_H_

// EOF