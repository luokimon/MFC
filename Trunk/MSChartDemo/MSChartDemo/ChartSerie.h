#pragma once

class CChartCtrl;

class CChartSerie
{
public:
	//CChartSerie();
	CChartSerie(CChartCtrl* pParent);
	virtual ~CChartSerie();

protected:
	bool m_bIsVisible;
	COLORREF m_SerieColor;
	CRect m_PlottingRect;
	CChartCtrl* m_pParentCtrl;
	CString m_strSerieName;

protected:
	bool IsNotifyMouseClickEnable() { return m_bMouseClickNotifications; }
	bool IsNotifyMouseMoveEnable() { return m_bMouseMoveNotifications; }

private:
	bool m_bMouseClickNotifications;
	bool m_bMouseMoveNotifications;

public:
	void EnableMouseNotifications(bool bClickEnabled, bool bMoveEnabled);
	void SetName(const CString& name) { m_strSerieName = name; }
	CString GetName() const { return m_strSerieName; }
};

