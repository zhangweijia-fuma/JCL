#ifndef TEXTTICKER_H
#define TEXTTICKER_H

#include <QLabel>
#include "PISTypes.h"

class CScrollLabel : public QLabel
{
    Q_OBJECT

    // Operations
    public:
        CScrollLabel(QWidget *parent = 0);
        ~CScrollLabel();

        void                SetParam(QString strContent, int nShowMode = SCROLL_SHOW_SCROLL, int nLifeTime = 0, bool bIsChinese = true);
        bool                m_bIsInitialed;
        void                SetScroll(int nScrollMode) { m_nShowMode = nScrollMode;}

    protected:
        void                ShowScrollText();
        void                ShowFullScreenText();

    // Attributes
    protected:
        int                 m_nTextLen;                                 // Position
        QString             m_strSpace;
        int                 m_nStartPos;
        int                 m_nShowMode;                                // 显示模式，不滚动，滚动或全屏
        int                 m_nLifeTime;                                // 滚动消息的显示时间，单位：秒，0：为一直显示，默认为一直显示

        QString             m_strText;
        QTimer*             m_pTimer;

        bool                m_bIsChinese;
    private:

    private slots:
        void                UpdateText();
};

#endif // TEXTTICKER_H
