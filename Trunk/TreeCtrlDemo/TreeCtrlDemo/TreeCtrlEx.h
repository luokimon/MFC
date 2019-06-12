#pragma once

class CTreeCtrlEx : public CTreeCtrl
{
public:
	CTreeCtrlEx();
	virtual ~CTreeCtrlEx();

protected:
	UINT          m_TimerTicks;      //��������Ķ�ʱ����������ʱ��
	UINT          m_nScrollTimerID;  //��������Ķ�ʱ��
	CPoint        m_HoverPoint;      //���λ��
	UINT          m_nHoverTimerID;   //������ж�ʱ��
	DWORD         m_dwDragStart;     //������������һ�̵�ʱ��
	BOOL          m_bDragging;       //��ʶ�Ƿ������϶�������
	CImageList*   m_pDragImage;      //�϶�ʱ��ʾ��ͼ���б�
	HTREEITEM     m_hItemDragS;      //���϶��ı�ǩ
	HTREEITEM     m_hItemDragD;      //�����϶��ı�ǩ

protected:
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
private:
	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
public:
	void OnAddChild();
	void OnAddSibling();
	void OnDeleteItem();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	HTREEITEM CTreeCtrlEx::InsertItemAndSetImage(LPCTSTR lpszItem,
		HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};

