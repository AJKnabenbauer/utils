#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#define DIALOG_FMAX_SINGLE MAX_PATH //
#define DIALOG_FMAX_MULTI 32768 // 32k for multiselect 
#define DIALOG_FILETITLE_BUFFER_SIZE 256
#define DIALOG_FLAGS_SINGLE ( OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST )
#define DIALOG_FLAGS_MULTI ( OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER )


// These two helper functions can be used to create a const wchar_t string
// of filetype filters with the correct format for file dialogs.
// 
// Example file type filter:
// 
//	const wchar_t example[] = fileTypeFilterString(
//		fileTypeFilterValue("Windows Bitmap", "*.bmp;*.dib")
//		fileTypeFilterValue("Graphics Interchange Format", "*.gif")
//		fileTypeFilterValue("JPEG File Interchange Format", "*.jpg;*.jpeg")
//		fileTypeFilterValue("All Files", "*.*")
//	);
// 
// Use fileTypeFilterString() to wrap the entire filter and use 
// fileTypeFilterValue() for each filter option. 
// 
// The filter description parameter is what is shown in the drop down selection.
// 
// The filter extensions parameter should be formatted as *.[extention]. A * can be 
// used on the right of the period as a wildcard for all file extentions (see "All Files"
// in the above example). The string can contain multiple extensions seperated by a semicolon.
// 
// Strings should be passed as basic ASCII string literals. L prefixes are automatically added.
// 

// Appends an additional null terminator to the end of a filter string
#define fileTypeFilterString(x) (x L"\0")

// Takes a description of the filter, and a semicolon delimited list of file
// extensions and adds the necessary null terminators.
#define fileTypeFilterValue(description, extentions) L##description L"\0" L##extentions L"\0"

#define fileTypeFilterDefault fileTypeFilterString(fileTypeFilterValue("All Files", "*.*"))

typedef struct 
{
	wchar_t* strFile; // buffer to hold the overall file string
	size_t lenFile; // length in characters of buffer
	size_t offsetTitle;
	wchar_t* strTitle;
	bool multiSelect; // was the dialog multi select or not
	size_t fileCount;
	wchar_t* strCombBuffer; // buffer used to return full length strings from multi select dialog boxes. Will only be allocated if the dialog is multi select
	size_t lenCombBuffer;
} akDialogStruct;

void akDialog_free( _Inout_ akDialogStruct* ptr_struct );

bool akFile_open_dialog( bool multi_select, _Inout_ akDialogStruct* ptr_struct, _In_opt_ const wchar_t* str_filter, _In_opt_ const wchar_t* str_title );

const wchar_t* akDialog_get_title_string( _Inout_ akDialogStruct* ptr_struct, unsigned int index );

#ifdef __cplusplus
}
#endif

