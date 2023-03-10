#pragma once

#include <qsciscintilla.h>
#include <Scintilla.h>
#include <Platform.h>
#include <QDragEnterEvent>
#include <QDropEvent>

typedef sptr_t(*SCINTILLA_FUNC) (sptr_t ptr, unsigned int, uptr_t, sptr_t);
typedef sptr_t SCINTILLA_PTR;

class CCNotePad;
class ScintillaHexEditView : public QsciScintilla
{
	Q_OBJECT

public:
	ScintillaHexEditView(QWidget *parent = Q_NULLPTR);
	virtual ~ScintillaHexEditView();

	void setNoteWidget(QWidget * win);

	sptr_t execute(quint32 Msg, uptr_t wParam = 0, sptr_t lParam = 0) const;

	void updateThemes();

private:
	void init();

private slots:
	void slot_scrollYValueChange(int value);

protected:
	void dragEnterEvent(QDragEnterEvent * event);
	void dropEvent(QDropEvent * e);

	//! Re-implemented to handle mouse moves.
	virtual void mouseMoveEvent(QMouseEvent* e);

	//! Re-implemented to handle mouse presses.
	virtual void mousePressEvent(QMouseEvent* e);

	//! Re-implemented to handle mouse releases.
	virtual void mouseReleaseEvent(QMouseEvent* e);

private:
	static bool _SciInit;

	SCINTILLA_FUNC m_pScintillaFunc = nullptr;
	SCINTILLA_PTR  m_pScintillaPtr = 0;

	CCNotePad* m_NoteWin;

	bool selecting = false;

	Scintilla::Point m_SelFrom;
	Scintilla::Point m_SelTo;
};
