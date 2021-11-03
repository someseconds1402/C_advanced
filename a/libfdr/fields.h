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
#ifndef _FIELDS_ 
#define _FIELDS_ 

/* The fields library -- making input processing easier */

#include <stdio.h>
#define MAXLEN 1001
#define MAXFIELDS 1000

typedef struct inputstruct {
  char *name;               /* File name */
  FILE *f;                  /* File descriptor */
  int line;                 /* Line number */
  char text1[MAXLEN];       /* The line */
  char text2[MAXLEN];       /* Working -- contains fields */
  int NF;                   /* Number of fields */
  char *fields[MAXFIELDS];  /* Pointers to fields */
  int file;                 /* 1 for file, 0 for popen */
} *IS;

extern IS new_inputstruct(/* FILENAME -- NULL for stdin */);
extern IS pipe_inputstruct(/* COMMAND -- NULL for stdin */);
extern int get_line(/* IS */); /* returns NF, or -1 on EOF.  Does not
                                  close the file */
extern void jettison_inputstruct(/* IS */);  /* frees the IS and fcloses 
                                                the file */
#endif
/*
Để đọc tệp với thư viện trường, bạn gọi new_inputstruct () với tên tệp thích hợp. New_inputstruct () lấy tên tệp làm đối số của nó ( NULL cho đầu vào tiêu chuẩn) và kết quả là trả về IS . Lưu ý rằng IS là một con trỏ tới cấu trúc đầu vào . Đây là malloc () 'd cho bạn trong lệnh gọi new_inputstruct () . Nếu new_inputstruct () không thể mở tệp, nó sẽ trả về NULL và bạn có thể gọi perror () để in ra lý do cho sự thất bại (đọc trang man trên perror () nếu bạn muốn tìm hiểu về nó).

Khi bạn có IS , bạn gọi get_line () trên đó để đọc một dòng. Get_line () thay đổi trạng thái của IS để phản ánh cách đọc của dòng. Đặc biệt:

Nó đặt nội dung của dòng trong text1 .
Nó chia dòng thành lời. Trường NF chứa số lượng từ trong trường. Các khe NF đầu tiên của mảng trường trỏ đến từng từ NF (và những từ này được kết thúc bằng null).
Trường dòng chứa số dòng của dòng.
Get_line () trả về trường NF làm giá trị trả về của nó.
Nó trả về -1 khi đến cuối tập tin.
Jettison_inputstruct () đóng tệp được liên kết với IS và thỏa thuận (giải phóng) IS . Đừng lo lắng về pipe_inputstruct () .
*/
