
// MiniEditDoc.cpp : implementation of the CMiniEditDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MiniEdit.h"
#endif

#include "MiniEditDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMiniEditDoc

IMPLEMENT_DYNCREATE(CMiniEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CMiniEditDoc, CDocument)
END_MESSAGE_MAP()


// CMiniEditDoc construction/destruction

CMiniEditDoc::CMiniEditDoc() noexcept
{
	// TODO: add one-time construction code here

}

CMiniEditDoc::~CMiniEditDoc()
{
}

BOOL CMiniEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(nullptr);
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMiniEditDoc serialization

void CMiniEditDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
	}
#ifdef SHARED_HANDLERS

	if (m_viewList.IsEmpty() && ar.IsLoading())
	{
		CFile* pFile = ar.GetFile();
		pFile->Seek(0, FILE_BEGIN);
		ULONGLONG nFileSizeBytes = pFile->GetLength();
		ULONGLONG nFileSizeChars = nFileSizeBytes/sizeof(TCHAR);
		LPTSTR lpszText = (LPTSTR)malloc(((size_t)nFileSizeChars + 1) * sizeof(TCHAR));
		if (lpszText != nullptr)
		{
			ar.Read(lpszText, (UINT)nFileSizeBytes);
			lpszText[nFileSizeChars] = '\0';
			m_strThumbnailContent = lpszText;
			m_strSearchContent = lpszText;
		}
	}
#endif
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMiniEditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMiniEditDoc::InitializeSearchContent()
{
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// Use the entire text file content as the search content.
	SetSearchContent(m_strSearchContent);
}

void CMiniEditDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMiniEditDoc diagnostics

#ifdef _DEBUG
void CMiniEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMiniEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMiniEditDoc commands


void CMiniEditDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	POSITION Pos = GetFirstViewPosition();
	CEditView* PCEditView = (CEditView*)GetNextView(Pos);
	if (PCEditView)
		PCEditView->SetWindowText(_T(""));


	CDocument::DeleteContents();
}
