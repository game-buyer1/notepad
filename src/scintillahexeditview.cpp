#include "scintillahexeditview.h"
#include "styleset.h"

#include "ccnotepad.h"
#include <stdexcept>
#include <QMimeData>
#include <QScrollBar>

// initialize the static variable
bool ScintillaHexEditView::_SciInit = false;
#define DEFAULT_FONT_NAME "Courier New"

ScintillaHexEditView::ScintillaHexEditView(QWidget *parent):QsciScintilla(parent), m_NoteWin(nullptr)
{
	init();
}

ScintillaHexEditView::~ScintillaHexEditView()
{
}

void ScintillaHexEditView::setNoteWidget(QWidget * win)
{
	CCNotePad* pv = dynamic_cast<CCNotePad*>(win);
	if (pv != nullptr)
	{
		m_NoteWin = pv;
}
}

sptr_t ScintillaHexEditView::execute(quint32 Msg, uptr_t wParam, sptr_t lParam) const {
	try {
		return (m_pScintillaFunc) ? m_pScintillaFunc(m_pScintillaPtr, Msg, wParam, lParam) : -1;
	}
	catch (...)
	{
		return -1;
	}
};

void ScintillaHexEditView::init()
{
	if (!_SciInit)
	{
		_SciInit = true;
	}

	m_pScintillaFunc = (SCINTILLA_FUNC)this->SendScintillaPtrResult(SCI_GETDIRECTFUNCTION);
	m_pScintillaPtr = (SCINTILLA_PTR)this->SendScintillaPtrResult(SCI_GETDIRECTPOINTER);


	if (!m_pScintillaFunc)
	{
		throw std::runtime_error("ScintillaEditView::init : SCI_GETDIRECTFUNCTION message failed");
	}

	if (!m_pScintillaPtr)
	{
		throw std::runtime_error("ScintillaEditView::init : SCI_GETDIRECTPOINTER message failed");
	}


	//设置字体
#if defined (Q_OS_MAC)
    QFont font(DEFAULT_FONT_NAME, 12, QFont::Normal);
#else
	QFont font(DEFAULT_FONT_NAME, 11, QFont::Normal);
#endif
	setFont(font);
	setMarginsFont(font);
	execute(SCI_SETTABWIDTH, 4);

	updateThemes();

	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &ScintillaHexEditView::slot_scrollYValueChange);

}

//Y方向滚动条值变化后的槽函数
void ScintillaHexEditView::slot_scrollYValueChange(int value)
{
	if (value >= this->verticalScrollBar()->maximum())
	{
		if (m_NoteWin != nullptr)
		{
			m_NoteWin->showChangePageTips(this);
		}
	}
	else if (value == this->verticalScrollBar()->minimum())
	{
		if (m_NoteWin != nullptr)
		{
			m_NoteWin->showChangePageTips(this);
		}
	}
}


void ScintillaHexEditView::dragEnterEvent(QDragEnterEvent* event)
{
	//if (event->mimeData()->hasFormat("text/uri-list")) //只能打开文本文件
	//{
	//	event->accept(); //可以在这个窗口部件上拖放对象
	//}
	//else
	//{
	//	event->ignore();
	//}
	event->accept();
	}

void ScintillaHexEditView::dropEvent(QDropEvent* e)
{
	QList<QUrl> urls = e->mimeData()->urls();
	if (urls.isEmpty())
		return;

	CCNotePad* pv = dynamic_cast<CCNotePad*>(m_NoteWin);
	if (pv != nullptr)
		pv->receiveEditDrop(e);

	//qDebug() << ui.leftSrc->geometry() << ui.rightSrc->geometry() << QCursor::pos() << this->mapFromGlobal(QCursor::pos());
}

void ScintillaHexEditView::mouseMoveEvent(QMouseEvent* e)
{
	if (property("type").toInt() == HEX_TYPE && selecting)
	{
		long from, to;
		int r1, c1, r2, c2;
		int begin, end;
		const int col_hex_start = 9;
		const int col_hex_finish = 56;
		const int col_text_start = 57;
		const int col_text_finish = 72;
		const int col_num = 75;

		m_SelTo = Scintilla::Point::FromInts(e->x(), e->y());

		from = execute(SCI_POSITIONFROMPOINT, m_SelFrom.x, m_SelFrom.y);
		to = execute(SCI_POSITIONFROMPOINT, m_SelTo.x, m_SelTo.y);

		lineIndexFromPosition(from, &r1, &c1);
		lineIndexFromPosition(to, &r2, &c2);

		execute(SCI_CLEARSELECTIONS);

		if (r1 < 0 || c1 < 0 || r2 < 0 || c2 < 0)
			return;

		
		if (c1 < col_text_start || c1 > col_text_finish)
		{
			//选择二进制区域
			for (int i = r1; i <= r2; ++i)
			{
				//每行hex显示区域
				if (i == r1)
				{
					if (c1 < col_hex_start)
						begin = col_hex_start;
					else if (c1 > col_hex_finish)
						begin = col_hex_finish;
					else begin = c1 / 3 * 3;
				}
				else
					begin = col_hex_start;

				if (i == r2)
				{
					if (c2 < col_hex_start)
						end = col_hex_start;
					else if (c2 > col_hex_finish)
						end = col_hex_finish;
					else end = c2 / 3 * 3 + 2;
				}
				else
					end = col_hex_finish;


				for (int j = begin; j < end; j += 3)
				{
					execute(SCI_ADDSELECTION, i * col_num + j, i * col_num + j + 2);
					execute(SCI_ADDSELECTION, i * col_num + col_text_start + (j - col_hex_start) / 3,
						i * col_num + col_text_start + (j - col_hex_start) / 3 + 1);
				}
			}
		}
		else
		{
			//选择字符区域
			for (int i = r1; i <= r2; ++i)
			{
				//每行字符显示区域
				if (i == r1)
				{
					if (c1 < col_text_start)
						begin = col_text_start;
					else if (c1 > col_text_finish)
						begin = col_text_finish;
					else begin = c1;
				}
				else
					begin = col_text_start;

				if (i == r2)
				{
					if (c2 < col_text_start)
						end = col_text_start;
					else if (c2 > col_text_finish)
						end = col_text_finish;
					else end = c2;
				}
				else
					end = col_text_finish;


				for (int j = begin; j <= end; ++j)
				{
					execute(SCI_ADDSELECTION, i * col_num + col_hex_start + (j - col_text_start) * 3, 
						i * col_num + col_hex_start + (j - col_text_start) * 3 + 2);
					execute(SCI_ADDSELECTION, i * col_num + j, i * col_num + j + 1);
				}
			}
		}
		return;
	}
	QsciScintillaBase::mouseMoveEvent(e);
}

void ScintillaHexEditView::mousePressEvent(QMouseEvent* e)
{
	if (property("type").toInt() == HEX_TYPE && !selecting)
	{
		execute(SCI_CLEARSELECTIONS);
		selecting = true;
		m_SelFrom = Scintilla::Point::FromInts(e->x(), e->y());
		return;
	}
	QsciScintillaBase::mousePressEvent(e);
}

void ScintillaHexEditView::mouseReleaseEvent(QMouseEvent* e)
{
	if (property("type").toInt() == HEX_TYPE && selecting)
	{
		selecting = false;
		return;
	}
	QsciScintillaBase::mouseReleaseEvent(e);
}


void ScintillaHexEditView::updateThemes()
{
	//如果是黑色主题，则单独做一些风格设置
	setColor(StyleSet::s_global_style->default_style.fgColor);
	setMarginsBackgroundColor(StyleSet::s_global_style->line_number_margin.bgColor);
	setPaper(StyleSet::s_global_style->default_style.bgColor);
}

