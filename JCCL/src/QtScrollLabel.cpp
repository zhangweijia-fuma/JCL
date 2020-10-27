#include "ScrollLabel.h"
#include "PISTypes.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>


// 默认显示的滚动消息会一直显示，直到有新的消息内容或者被清除内容

CScrollLabel::CScrollLabel(QWidget *parent)
    : QLabel(parent)
{
    m_nStartPos     = 0;                                                    // 当前角码
    m_nTextLen      = 1;
    m_nShowMode     = SCROLL_SHOW_SCROLL;                                     // 默认是滚动
    m_bIsInitialed  = false;
    m_bIsChinese    = true;

    // m_pShowTimer    = NULL;

    setAlignment(Qt::AlignLeft);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(UpdateText()));
    m_pTimer->start(350);
}

CScrollLabel::~CScrollLabel()
{
}

void CScrollLabel::UpdateText()
{
    if ( m_nShowMode == SCROLL_SHOW_NONE )
        return;

    if ( m_bIsInitialed == false )
    {
        m_strText = text();
        m_bIsInitialed  = true;
        m_nStartPos     = 0;
        m_nTextLen      = 1;
        m_strSpace      = "";
    }

    if ( m_nShowMode == SCROLL_SHOW_SCROLL )
        ShowScrollText();
    else
        ShowFullScreenText();
}

void CScrollLabel::ShowScrollText()
{
    if ( m_strText.length() == 0 )
        return;

    int nStep = 1;
    // if ( m_bIsChinese == false)
    //     nStep = 3;

    if ( m_nStartPos == 0 )
        m_nTextLen += nStep;
    else
        m_nStartPos += nStep;

    if ( m_nStartPos >= m_strText.length())
    {
        m_nStartPos = 0;
        m_nTextLen = 1;
    }

    QString strTitle = m_strText.mid(m_nStartPos, m_nTextLen);

    // 消息内容还没有完全显示完全，不需要加空格
    if ( !m_strText.endsWith(strTitle) )
    {
        setText(strTitle);
        return;
    }

    m_strSpace += "  "; //QString::fromUtf8(" ");

    strTitle += m_strSpace;

    setText(strTitle);

    // 显示的空格宽度已经大于屏幕宽度，则从新开始显示消息
    int nWidth = width();
    int nSpaceWidth = fontMetrics().width(m_strSpace);
    if ( nSpaceWidth >= nWidth )
    {
        m_nStartPos = 0;
        m_nTextLen  = 1;
        m_strSpace  = "";
        return;
    }
}

void CScrollLabel::ShowFullScreenText()
{
    // 默认显示的内容能够在一个屏幕上全部显示完，不需要上下滚动
}

void CScrollLabel::SetParam(QString strContent, int nShowMode, int nLifeTime, bool bIsChinese)
{
    setText(strContent);
    m_bIsInitialed  = false;

    m_bIsInitialed  = true;
    m_strText       = strContent;
    m_nLifeTime     = nLifeTime;
    m_bIsChinese    = bIsChinese;

    m_nStartPos     = 0;
    m_nTextLen      = 1;

    if ( bIsChinese == false )
    {
        m_pTimer->stop();
        delete m_pTimer;
        m_pTimer = new QTimer(this);
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(UpdateText()));
        m_pTimer->start(100);
    }

    m_nShowMode = nShowMode;
    if ( nShowMode != SCROLL_SHOW_FULL_SCREEN )
    {
        setWordWrap(false);
        setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    }
    else
    {
        setWordWrap(true);
        setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}
