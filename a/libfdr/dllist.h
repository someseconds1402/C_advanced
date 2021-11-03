/*
Libraries for fields, doubly-linked lists and red-black trees.
Copyright (C) 2001 James S. Plank

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

---------------------------------------------------------------------------
Please see http://www.cs.utk.edu/~plank/plank/classes/cs360/360/notes/Libfdr/
for instruction on how to use this library.

Jim Plank
plank@cs.utk.edu
http://www.cs.utk.edu/~plank

Associate Professor
Department of Computer Science
University of Tennessee
203 Claxton Complex
1122 Volunteer Blvd.
Knoxville, TN 37996-3450

     865-974-4397
Fax: 865-974-4404
*/
#ifndef _DLLIST_H_
#define _DLLIST_H_

#include "jval.h"

typedef struct dllist {
  struct dllist *flink;
  struct dllist *blink;
  Jval val;
} *Dllist;

extern Dllist new_dllist();
extern free_dllist(Dllist);

extern dll_append(Dllist, Jval);//them vao cuoi danh sach
extern dll_prepend(Dllist, Jval);//them vao dau danh sach
extern dll_insert_b(Dllist, Jval);//them 1 nut vao truoc nut duoc chi dinh, gia tri nut nay la val
extern dll_insert_a(Dllist, Jval);//cung the nhung nguoc lai

extern dll_delete_node(Dllist);
extern int dll_empty(Dllist);//return 1 if is empty

extern Jval dll_val(Dllist);

#define dll_first(d) ((d)->flink)
#define dll_next(d) ((d)->flink)
#define dll_last(d) ((d)->blink)
#define dll_prev(d) ((d)->blink)
#define dll_nil(d) (d)

#define dll_traverse(ptr, list) \
  for (ptr = list->flink; ptr != list; ptr = ptr->flink)
#define dll_rtraverse(ptr, list) \
  for (ptr = list->blink; ptr != list; ptr = ptr->blink)

#endif

/*
<>DLList new_dllist () : Phân bổ và trả về một danh sách liên kết đôi mới.
<>free_dllist (DLList l) : Phá hủy danh sách, gọi free () trên tất cả bộ nhớ được phân bổ trong danh sách. Danh sách không phải trống.
<>dll_prepend (DLList l, Jval val) : Thêm một nút mới ở đầu danh sách. Giá trị của nút này là val . DLL_prepend () không có giá trị trả về.
<>dll_append (DLList l, Jval val) : Thêm một nút mới ở cuối danh sách. Giá trị của nút này là val . DLL_append () không có giá trị trả về.
<>dll_insert_b (DLList n, Jval val) : Thêm một nút mới vào danh sách ngay trước nút được chỉ định. Giá trị của nút này là val .
<>dll_insert_a (DLList n, Jval val) : Thêm một nút mới vào danh sách ngay sau nút được chỉ định. Giá trị của nút này là val .
<>DLList dll_nil (DLList l) : Trả về một con trỏ tới nút nil (sentinel) cho danh sách. Bạn có thể nghĩ nil là một nút không có giá trị, bắt đầu và kết thúc danh sách. Vì l trỏ đến nút sentinel, dll_nil trả về l . Bạn không cần gọi dll_nil () để truy cập nút sentinel. Bạn chỉ có thể sử dụng l , mặc dù nó làm cho mã của bạn dễ đọc hơn nếu bạn sử dụng dll_nil () .
<>DLList dll_first (DLList l) : Trả về một con trỏ tới nút đầu tiên trong danh sách. Nếu danh sách trống, cái này trả về sentinel. Như với dll_nil () , bạn không cần gọi dll_first (l) - bạn chỉ có thể sử dụng l-> flink .
<>DLList dll_last (DLList l) : Trả về một con trỏ đến nút cuối cùng trong danh sách. Nếu danh sách trống, cái này trả về sentinel. Như với dll_nil () , bạn không cần gọi dll_last (l) - bạn chỉ cần sử dụng l-> nháy .
<>DLList dll_next (DLList n) : Trả về một con trỏ tới nút tiếp theo trong danh sách sau n . Nếu n là nút cuối cùng trong danh sách, thì dll_next (n) trả về sentinel. Như với dll_first () , bạn không cần gọi dll_next (n) - bạn chỉ có thể sử dụng n-> flink .
<>DLList dll_prev (DLList n) : Trả về một con trỏ tới nút trước đó trong danh sách trước n . Nếu n là nút đầu tiên trong danh sách, thì dll_prev (n) trả về sentinel. Như với dll_last () , bạn không cần gọi dll_prev (n) - bạn chỉ cần sử dụng n-> nháy .
<>int dll_empty (DLList l) : Trả về cho dù l trống.
<>Jval dll_val (DLList n) : Trả về trường val của nút n . Một lần nữa, bạn không cần phải sử dụng nó, nhưng đôi khi nó có ích.
<>int dll_delete_node (DLList n) : Xóa và giải phóng nút n .
Cuối cùng, có hai macro để duyệt qua dllists tiến và lùi. ptr nên là một DLL và danh sách nên là một DLL :
#define dll_traverse (ptr, danh sách) \
  cho (ptr = (danh sách) -> flink; ptr! = (danh sách); ptr = ptr-> flink)
#define dll_rtraverse (ptr, danh sách) \
  cho (ptr = (danh sách) -> nhấp nháy; ptr! = (danh sách); ptr = ptr-> nhấp nháy)
*/
