//-----------------------------------------------------------------------------
// Copyright © 2003 - Philip Howard - All rights reserved
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
//-----------------------------------------------------------------------------
// package	vrb
// homepage	http://phil.ipal.org/freeware/vrb/
//-----------------------------------------------------------------------------
// author	Philip Howard
// email	vrb at ipal dot org
// homepage	http://phil.ipal.org/
//-----------------------------------------------------------------------------
// This file is best viewed using a fixed spaced font such as Courier
// and in a display at least 120 columns wide.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// file		vrb.h
//
// purpose	Define resources used by and with the vrb library.
//-----------------------------------------------------------------------------
#ifndef __VRB_H__
#define __VRB_H__
#include <stdlib.h>
//-----------------------------------------------------------------------------
// struct	vrb
// typedef	VRB, vrb_p
//
// purpose	This is the vrb header structure used to hold the state
//		of this instance of a vrb.
//-----------------------------------------------------------------------------
struct vrb {
    char *			lower_ptr	;
    char *			upper_ptr	;
    char *			first_ptr	;
    char *			last_ptr	;
    char *			mem_ptr		;
    size_t			buf_size	;
    int				flags		;
};
typedef struct vrb		VRB		;
typedef VRB *			vrb_p		;

#define VRB_FLAG_SHMAT		0x0001
#define VRB_FLAG_MMAP		0x0002
#define VRB_FLAG_ERROR		0x0004
#define VRB_FLAG_GUARD		0x0008

#define vrb_flag_shmat(b)	(((b)->flags)&(VRB_FLAG_SHMAT))
#define vrb_flag_mmap(b)	(((b)->flags)&(VRB_FLAG_MMAP))
#define vrb_flag_error(b)	(((b)->flags)&(VRB_FLAG_ERROR))
#define vrb_flag_guard(b)	(((b)->flags)&(VRB_FLAG_GUARD))

#define vrb_is_shmat(b)		(!(!(((b)->flags)&(VRB_FLAG_SHMAT))))
#define vrb_is_mmap(b)		(!(!(((b)->flags)&(VRB_FLAG_MMAP))))
#define vrb_is_error(b)		(!(!(((b)->flags)&(VRB_FLAG_ERROR))))
#define vrb_is_guard(b)		(!(!(((b)->flags)&(VRB_FLAG_GUARD))))

#define vrb_is_not_shmat(b)	(!(((b)->flags)&(VRB_FLAG_SHMAT)))
#define vrb_is_not_mmap(b)	(!(((b)->flags)&(VRB_FLAG_MMAP)))
#define vrb_is_not_error(b)	(!(((b)->flags)&(VRB_FLAG_ERROR)))
#define vrb_is_not_guard(b)	(!(((b)->flags)&(VRB_FLAG_GUARD)))

#define vrb_set_shmat(b)	(((b)->flags)|=(VRB_FLAG_SHMAT))
#define vrb_set_mmap(b)		(((b)->flags)|=(VRB_FLAG_MMAP))
#define vrb_set_error(b)	(((b)->flags)|=(VRB_FLAG_ERROR))
#define vrb_set_guard(b)	(((b)->flags)|=(VRB_FLAG_GUARD))

#define vrb_unset_shmat(b)	(((b)->flags)&=(~(VRB_FLAG_SHMAT)))
#define vrb_unset_mmap(b)	(((b)->flags)&=(~(VRB_FLAG_MMAP)))
#define vrb_unset_error(b)	(((b)->flags)&=(~(VRB_FLAG_ERROR)))
#define vrb_unset_guard(b)	(((b)->flags)&=(~(VRB_FLAG_GUARD)))

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// macro	VRB_NOGUARD
//
// purpose	Option flag bit to disable guard pages
//-----------------------------------------------------------------------------
#define VRB_NOGUARD	0x0001

//-----------------------------------------------------------------------------
// macro	VRB_ENVGUARD
//
// purpose	Option flag bit to override VRB_NOGUARD when the environment
//		variable VRBGUARD is defined to a value other than "0"
//-----------------------------------------------------------------------------
#define VRB_ENVGUARD	0x0002

//-----------------------------------------------------------------------------
// macro	VRB_NOMKSTEMP
//
// purpose	Option flag bit to disable using mkstemp() to make a file name
//		from the given name pattern.  With this option, the name given
//		is used as an absolute name.
//-----------------------------------------------------------------------------
#define VRB_NOMKSTEMP	0x0004

//-----------------------------------------------------------------------------
// macro	vrb_capacity
//
// purpose	Return the capacity of the specified vrb.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(size_t) capacity of buffer
//-----------------------------------------------------------------------------
#define vrb_capacity(b) ((b)->buf_size)

//-----------------------------------------------------------------------------
// macro	vrb_data_len
//
// purpose	Return the current data length of the specified vrb.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(size_t) current data length in buffer
//-----------------------------------------------------------------------------
#define vrb_data_len(b) (((b)->last_ptr)-((b)->first_ptr))

//-----------------------------------------------------------------------------
// macro	vrb_data_ptr
//
// purpose	Get the pointer to data contents in the specified buffer.
//
//		Along with vrb_data_len(), this allows the caller direct
//		access to the data contents to determine how much to take,
//		and to call vrb_take() to specify how much data is taken.
//
// arguments	1 (vrb_p) pointer to vrb
//
// returns	(char *) pointer to data in buffer
//
// note		The data pointer only has meaning if there is data in the
//		buffer.	 Use vrb_data_len() to get the length to determine
//		how much data may be examined.
//-----------------------------------------------------------------------------
#define vrb_data_ptr(b) ((b)->first_ptr)

//-----------------------------------------------------------------------------
// macro	vrb_data_end
//
// purpose	Get the pointer to the end of data contents in the specified
//		buffer.	 This can be used by the caller to test a moving
//		pointer that was started from vrb_data_ptr() to scan data.
//
//		Along with vrb_data_ptr(), the allows the caller direct
//		access to the data contents to determine how much to take,
//		and to call vrb_take() to specify how much data is taken.
//
// arguments	1 (vrb_p) pointer to vrb
//
// returns	(char *) pointer to end of data in buffer
//
// note		The end of data pointer is always one past the last byte of
//		data, and cannot be used directly except to compare another
//		data pointer.
//
// note		The value of vrb_data_end() will be the same as vrb_data_ptr()
//		when the buffer is empty.
//-----------------------------------------------------------------------------
#define vrb_data_end(b) ((b)->last_ptr)

//-----------------------------------------------------------------------------
// macro	vrb_space_len
//
// purpose	Return the length of available space in the specified vrb.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(size_t) length of available space
//-----------------------------------------------------------------------------
#define vrb_space_len(b) (((b)->first_ptr)+((b)->buf_size)-((b)->last_ptr))

//-----------------------------------------------------------------------------
// macro	vrb_space_ptr
//
// purpose	Get the pointer to empty space in the specified buffer.
//
//		Along with vrb_space_len(), this allows the caller direct
//		access to the empty space to place data into the buffer,
//		and to call vrb_give() to specify home data is given.
//
// arguments	1 (vrb_p) pointer to vrb
//
// returns	(char *) pointer to space in buffer
//
// note		The space pointer only has meaning if there is space in the
//		buffer.	 Use vrb_available() to get the length to determine
//		how much data may be put in the buffer.
//-----------------------------------------------------------------------------
#define vrb_space_ptr(b) ((b)->last_ptr)

//-----------------------------------------------------------------------------
// macro	vrb_space_end
//
// purpose	Get the pointer to the end of empty space in the specified
//		buffer.
//
//		Along with vrb_space_ptr(), this allows the caller direct
//		access to the empty space to place data into the buffer,
//		and to call vrb_give() to specify home data is given.
//
// arguments	1 (vrb_p) pointer to vrb
//
// returns	(char *) pointer to end of space in buffer
//
// note		The end of data pointer is always one past the last byte of
//		space, and cannot be used directly except to compare another
//		space pointer.
//-----------------------------------------------------------------------------
#define vrb_space_end(b) (((b)->first_ptr)+((b)->buf_size))

//-----------------------------------------------------------------------------
// macro	vrb_is_empty
//
// purpose	Return true if the specified vrb is currently empty, else
//		return false.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(int) false : buffer is not empty
//		(int) true  : buffer is empty
//-----------------------------------------------------------------------------
#define vrb_is_empty(b) (((b)->last_ptr)==((b)->first_ptr))

//-----------------------------------------------------------------------------
// macro	vrb_is_full
//
// purpose	Return true if the specified vrb is currently full, else
//		return false.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(int) false : buffer is not full
//		(int) true  : buffer is full
//-----------------------------------------------------------------------------
#define vrb_is_full(b) (vrb_space_len((b))==0)

//-----------------------------------------------------------------------------
// macro	vrb_is_not_empty
//
// purpose	Return true if the specified vrb is currently empty, else
//		return false.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(int) false : buffer is not empty
//		(int) true  : buffer is empty
//-----------------------------------------------------------------------------
#define vrb_is_not_empty(b) (!(vrb_is_empty((b))))

//-----------------------------------------------------------------------------
// macro	vrb_is_not_full
//
// purpose	Return true if the specified vrb is currently full, else
//		return false.
//
// arguments	1 (vrb_p) pointer to the vrb to examine
//
// returns	(int) false : buffer is not full
//		(int) true  : buffer is full
//-----------------------------------------------------------------------------
#define vrb_is_not_full(b) (!(vrb_is_full((b))))
//-----------------------------------------------------------------------------
// macro	vrb_new
//
// purpose	Make a new instance of a vrb, returning the pointer to the
//		vrb structure.
//
//		If no name is given, use SysV shared memory for the shared
//		memory object, and swap for backing store.
//
//		If a file name pattern is given, use POSIX memory mapping
//		into a temporary file created from that file name pattern
//		for the shared memory object, using file for backing store.
//
// arguments	1 (size_t) minimum buffer space requested
//		2 (const char *) name (temp pattern or actual) to memory map
//
// note		The name argument is a temporary filename pattern used by
//		mkstemp() to create a unique temporary file if it ends with
//		the string "XXXXXX" as required by mkstemp(), or is used as
//		an actual filename if it does not end with that string.
//
// returns	(vrb_p) pointer to vrb
//		(vrb_p) NULL if error (see errno)
//-----------------------------------------------------------------------------
#define vrb_new(s,n) (vrb_init_opt(((vrb_p)(NULL)),(s),(n),0))
//-----------------------------------------------------------------------------
// macro	vrb_new_opt
//
// purpose	Make a new instance of a vrb, returning the pointer to the
//		vrb structure.
//
//		This version includes an option flags argument.
//
//		If no name is given, use SysV shared memory for the shared
//		memory object, and swap for backing store.
//
//		If a file name pattern is given, use POSIX memory mapping
//		into a temporary file created from that file name pattern
//		for the shared memory object, using file for backing store.
//
// arguments	1 (size_t) minimum buffer space requested
//		2 (const char *) name (temp pattern or actual) to memory map
//		3 (int) option flags
//
// options	VRB_NOGUARD	Do not add guard pages around the buffer
//
//		VRB_ENVGUARD	Override VRB_NOGUARD if the environment
//				variable VRBGUARD is defined other than
//				the string "0"
//
//		VRB_NOMKSTEMP	Do not use mkstemp() to make a temporary
//				file name from the given pattern.  Use it
//				directly as the file name.
//
// note		The name argument is a temporary filename pattern used by
//		mkstemp() to create a unique temporary file if it ends with
//		the string "XXXXXX" as required by mkstemp(), or is used as
//		an actual filename if it does not end with that string.
//
// returns	(vrb_p) pointer to vrb
//		(vrb_p) NULL if error (see errno)
//-----------------------------------------------------------------------------
#define vrb_new_opt(s,n,o) (vrb_init_opt(((vrb_p)(NULL)),(s),(n),(o)))
//-----------------------------------------------------------------------------
// macro	vrb_init
//
// purpose	Initialize a vrb with an empty space of at least the size
//		requested.  The actual size will be rounded up to meet system
//		requirements, if needed.  Additional address space will be
//		used for guard pages added to prevent buffer overflow errors.
//		
//		If no vrb is given, create a new one.
//
//		If no name is given, use SysV shared memory for the shared
//		memory object, and swap for backing store.
//
//		If a file name pattern is given, use POSIX memory mapping
//		into a temporary file created from that file name pattern
//		for the shared memory object, using file for backing store.
//
// arguments	1 (vrb_p) NULL or an existing vrb to initialize
//		2 (size_t) buffer size, which will be rounded up as needed
//		3 (const char *) name (temp pattern or actual) to memory map
//
// note		The name argument is a temporary filename pattern used by
//		mkstemp() to create a unique temporary file if it ends with
//		the string "XXXXXX" as required by mkstemp(), or is used as
//		an actual filename if it does not end with that string.
//
// returns	(vrb_p) pointer to initialized vrb
//		(vrb_p) NULL if there was an error
//-----------------------------------------------------------------------------
#define vrb_init(v,s,n) (vrb_init_opt((v),(s),(n),0))
//-----------------------------------------------------------------------------
// function	vrb_destroy
//
// purpose	Destroy an instance of a vrb.
//
// arguments	1 (vrb_p) pointer to vrb to destroy
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//-----------------------------------------------------------------------------
int
vrb_destroy (
    vrb_p	arg_vrb
    )
;

//-----------------------------------------------------------------------------
// function	vrb_empty
//
// purpose	Discard all the data in a buffer, making it empty.
//
// arguments	1 (vrb_p) pointer to vrb
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//-----------------------------------------------------------------------------
int
vrb_empty (
    vrb_p	arg_vrb
    )
;

//-----------------------------------------------------------------------------
// function	vrb_get
//
// purpose	Get data from a vrb and copy it to a caller provided space.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (char *) pointer to where to copy data
//		3 (size_t) size of data requested
//
// returns	(size_t) size of data copied to caller space
//		(size_t) ~0 : error
//-----------------------------------------------------------------------------
size_t
vrb_get (
    vrb_p	arg_vrb
    ,
    char *	arg_data
    ,
    size_t	arg_size
    )
;

//-----------------------------------------------------------------------------
// function	vrb_get_min
//
// purpose	Get a minimum amount of data from a vrb and copy it to a
//		caller provided space.	If the minimum request cannot be
//		fulfilled, nothing is copied.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (char *) pointer to where to copy data
//		3 (size_t) minimum size requested
//		4 (size_t) maximum size requested
//
// returns	(size_t) size of data copied to caller space
//		(size_t) ~0 : error
//-----------------------------------------------------------------------------
size_t
vrb_get_min (
    vrb_p	arg_vrb
    ,
    char *	arg_data
    ,
    size_t	arg_min_size
    ,
    size_t	arg_max_size
    )
;

//-----------------------------------------------------------------------------
// function	vrb_give
//
// purpose	Specify how much data is given to the specified buffer and
//		update pointers to reflect this change.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (size_t) size that was given
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//-----------------------------------------------------------------------------
int
vrb_give (
    vrb_p	arg_vrb
    ,
    size_t	arg_size
    )
;

//-----------------------------------------------------------------------------
// function	vrb_init_opt
//
// purpose	Initialize a vrb with an empty space of at least the size
//		requested.  The actual size will be rounded up to meet system
//		requirements, if needed.  Additional address space will be
//		used for guard pages added to prevent buffer overflow errors.
//
//		This version includes an option flags argument.
//		
//		If no vrb is given, create a new one.
//
//		If no name is given, use SysV shared memory for the shared
//		memory object, and swap for backing store.
//
//		If a file name pattern is given, use POSIX memory mapping
//		into a temporary file created from that file name pattern
//		for the shared memory object, using file for backing store.
//
// arguments	1 (vrb_p) NULL or an existing vrb to initialize
//		2 (size_t) buffer size, which will be rounded up as needed
//		3 (const char *) name (temp pattern or actual) to memory map
//		4 (int) option flags
//
// options	VRB_NOGUARD	Do not add guard pages around the buffer
//
//		VRB_ENVGUARD	Override VRB_NOGUARD if the environment
//				variable VRBGUARD is defined other than
//				the string "0"
//
//		VRB_NOMKSTEMP	Do not use mkstemp() to make a temporary
//				file name from the given pattern.  Use it
//				directly as the file name.
//
// note		The name argument is a temporary filename pattern used by
//		mkstemp() to create a unique temporary file if it ends with
//		the string "XXXXXX" as required by mkstemp(), or is used as
//		an actual filename if it does not end with that string.
//
// returns	(vrb_p) pointer to initialized vrb
//		(vrb_p) NULL if there was an error
//-----------------------------------------------------------------------------
vrb_p
vrb_init_opt (
    vrb_p		arg_vrb
    ,
    size_t		arg_size
    ,
    const char *	arg_name
    ,
    int			arg_option
    )
;

//-----------------------------------------------------------------------------
// function	vrb_move
//
// purpose	Move data from one buffer to another buffer, limited only by
//		how much data there is in the source and how much space there
//		in the destination, or the specified amount.
//
// arguments	1 (vrb_p) pointer to destination vrb
//		2 (vrb_p) pointer to source vrb
//		3 (size_t) max length to move or ~0 for all
//
// returns	(size_t) size of data moved between buffers
//		(size_t) ~0 : error
//-----------------------------------------------------------------------------
size_t
vrb_move (
    vrb_p	arg_vrb_dst
    ,
    vrb_p	arg_vrb_src
    ,
    size_t	arg_max_len
    )
;

//-----------------------------------------------------------------------------
// function	vrb_put
//
// purpose	Put caller provided data into the buffer as input.
//		Put in as much as possible if not all can fit.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (char *) pointer to data to put into the buffer
//		3 (size_t) size of data to put into the buffer
//
// returns	(size_t) amount of data actually put in
//		(size_t) ~0 : error
//-----------------------------------------------------------------------------
size_t
vrb_put (
    vrb_p	arg_vrb
    ,
    char *	arg_data
    ,
    size_t	arg_size
    )
;

//-----------------------------------------------------------------------------
// function	vrb_put_all
//
// purpose	Put caller provided data into the buffer as input only if all
//		of the data will fit.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (char *) pointer to data to put into the buffer
//		3 (size_t) size of data to put into the buffer
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//-----------------------------------------------------------------------------
int
vrb_put_all (
    vrb_p	arg_vrb
    ,
    char *	arg_data
    ,
    size_t	arg_size
    )
;

//-----------------------------------------------------------------------------
// macro	vrb_read_min
//
// purpose	Read some data from a file descriptor into a vrb only if
//		enough space is available to read a specified minimum amount.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (int) file descriptor
//		3 (size_t) maximum size to read or ~0 for no limit
//		4 (size_t) minimum size to read
//
// returns	(size_t)  >  0 : size actually read into vrb
//		(size_t) ==  0 : insufficient space to read into
//		(size_t) == ~0 && errno == 0 : end of file in read()
//		(size_t) == ~0 && errno != 0 : error from read(), see errno
//
// note		The EOF return semantics are different than for read().
//-----------------------------------------------------------------------------
#define vrb_read_min(b,f,x,m) (((m)<(vrb_space_len((b))))?0:(vrb_read((b),(f),(x))))

//-----------------------------------------------------------------------------
// function	vrb_read
//
// purpose	Read some data from a file descriptor into a vrb.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (int) file descriptor
//		3 (size_t) maximum size to read or ~0 for no limit
//
// returns	(size_t)  >  0 : size actually read into vrb
//		(size_t) ==  0 : no space to read into
//		(size_t) == ~0 && errno == 0 : end of file in read()
//		(size_t) == ~0 && errno != 0 : error from read(), see errno
//
// note		The EOF return semantics are different than for read().
//-----------------------------------------------------------------------------
size_t
vrb_read (
    vrb_p        arg_vrb
    ,
    int		arg_fd
    ,
    size_t	arg_size
    )
;

//-----------------------------------------------------------------------------
// function	vrb_resize
//
// purpose	Change the size of the buffer space in a VRB.
//
// arguments	1 (vrb_p) pointer to vrb to resize
//		2 (size_t) new size, which will be rounded up as needed
//		3 (const char *) filename to memory map into
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//
// note		This operation uses a lot of resources, so it should be used
//		only sparingly.
//-----------------------------------------------------------------------------
int
vrb_resize (
    vrb_p		arg_vrb
    ,
    size_t		arg_size
    ,
    const char *	arg_name
    )
;

//-----------------------------------------------------------------------------
// function	vrb_take
//
// purpose	Specify how much data is taken from the specified buffer and
//		update pointers to reflect this change.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (size_t) size that has been taken
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//-----------------------------------------------------------------------------
int
vrb_take (
    vrb_p	arg_vrb
    ,
    size_t	arg_size
    )
;

//-----------------------------------------------------------------------------
// function	vrb_uninit
//
// purpose	UN-initialize a vrb, releasing its buffer memory.
//
// arguments	1 (vrb_p) pointer to vrb to un-initialize
//
// returns	(int) == 0 : OK
//		(int)  < 0 : error
//-----------------------------------------------------------------------------
int
vrb_uninit (
    vrb_p	arg_vrb
    )
;

//-----------------------------------------------------------------------------
// macro	vrb_write_min
//
// purpose	Write some data from a vrb to a file descriptor only if
//		enough data is available to write a specified minimum amount.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (int) file descriptor
//		3 (size_t) maximum size to write or ~0 for no limit
//		4 (size_t) minimum size to write
//
// returns	(size_t)  >  0 : size actually written
//		(size_t) ==  0 : insufficient data in buffer
//		(size_t) == ~0 : error from write(), see errno
//-----------------------------------------------------------------------------
#define vrb_write_min(b,f,x,m) (((m)<(vrb_data_len((b))))?0:(vrb_write((b),(f),(x))))

//-----------------------------------------------------------------------------
// function	vrb_write
//
// purpose	Write some data from a vrb to a file descriptor.
//
// arguments	1 (vrb_p) pointer to vrb
//		2 (int) file descriptor
//		3 (size_t) maximum size to write or ~0 for no limit
//
// returns	(size_t)  >  0 : size actually written
//		(size_t) ==  0 : no data in buffer
//		(size_t) == ~0 : error from write(), see errno
//-----------------------------------------------------------------------------
size_t
vrb_write (
    vrb_p	arg_vrb
    ,
    int		arg_fd
    ,
    size_t	arg_size
    )
;

#endif /* __VRB_H__ */
