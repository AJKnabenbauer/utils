#include "ak_files.h"

#include <string.h>
#include <Windows.h>
#include <commdlg.h>

void akDialog_free( _Inout_ akDialogStruct* ptr_struct )
{
	if ( ptr_struct->strFile != NULL ) {
		free( ptr_struct->strFile );
		ptr_struct->strFile = NULL;
	}

	if ( ptr_struct->strCombBuffer != NULL ) {
		free( ptr_struct->strCombBuffer );
		ptr_struct->strCombBuffer = NULL;
	}

	memset( ptr_struct, 0, sizeof( *ptr_struct ) );
}

bool akFile_open_dialog( bool multi_select, _Inout_ akDialogStruct* ptr_struct,
	_In_opt_ const wchar_t* str_filter, _In_opt_ const wchar_t* str_title )
{
	akDialogStruct tStruct = { 0 };
	tStruct.multiSelect = multi_select;
	tStruct.fileCount = multi_select ? 0 : 1;
	tStruct.lenFile = tStruct.multiSelect ? DIALOG_FMAX_MULTI : DIALOG_FMAX_SINGLE;
	tStruct.strFile = ( wchar_t* )calloc( tStruct.lenFile, sizeof( wchar_t ) );
	if ( !tStruct.strFile ) {
		return false;
	}

	// allocate a buffer to allow us to return concatenated long paths from multi 
	// selection dialog boxes
	if ( multi_select ) {
		tStruct.lenCombBuffer = MAX_PATH;
		tStruct.strCombBuffer = ( wchar_t* )calloc( tStruct.lenCombBuffer, sizeof( wchar_t ) );
		if ( !tStruct.strCombBuffer ) {
			akDialog_free( &tStruct );
			return false;
		};
	}

	// Show dialog box
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof( ofn );
	ofn.lpstrFilter = str_filter;
	ofn.lpstrTitle = str_title;
	ofn.lpstrFile = tStruct.strFile;
	ofn.nMaxFile = tStruct.lenFile;
	ofn.Flags = multi_select ? DIALOG_FLAGS_MULTI : DIALOG_FLAGS_SINGLE;
	if ( GetOpenFileName( &ofn ) != TRUE ) {
		akDialog_free( &tStruct );
		return false;	
	}

	// Find total length of file path string
	if ( multi_select ) {
		wchar_t* ptr = &tStruct.strFile[ofn.nFileOffset];
		while ( *ptr ) {
			tStruct.fileCount++;
			ptr = ptr + wcslen( ptr ) + 2;
		}
		tStruct.lenFile = ptr - tStruct.strFile;
	}
	else {
		tStruct.lenFile = wcslen( tStruct.strFile ) + 2;
	}

	// Re-size buffer to minimum needed
	wchar_t* ptr = ( wchar_t* )realloc( tStruct.strFile, tStruct.lenFile * sizeof( wchar_t ) );
	if ( !ptr ) {
		akDialog_free( &tStruct );
		return false;
	};

	tStruct.strFile = ptr;
	tStruct.offsetTitle = ofn.nFileOffset;
	tStruct.strTitle = &tStruct.strFile[tStruct.offsetTitle];

	*ptr_struct = tStruct;

	return true;
}

const wchar_t* akDialog_get_title_string( _Inout_ akDialogStruct* ptr_struct, unsigned int index )
{
	static const wchar_t nullStr = L'\0';

	if ( !ptr_struct->strFile || index >= ptr_struct->fileCount )
		return &nullStr;

	wchar_t* ptr = &ptr_struct->strFile[ptr_struct->offsetTitle];

	for ( int i = 0; i < index; i++ ) {
		if ( *ptr  )
			ptr = ptr + wcslen( ptr ) + 1;
		else 
			return &nullStr;
	}

	return ptr;
}




