#pragma once
#include <qwidget.h>
#include <qslider.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qboxlayout.h>
#include <qpainter.h>
#include <qevent.h>
#include <qpushbutton.h>
class ColorHueBar : public QWidget
{
	Q_OBJECT

public:
	ColorHueBar(QWidget *parent = nullptr);
	~ColorHueBar();

	void setValue(int val);
	int value();

signals:
	void sig_valueChanged(int val);

private:
	QSlider* m_slider;
};

class ColorSVCanvas : public QWidget
{
	Q_OBJECT

public:
	ColorSVCanvas(QWidget *parent = nullptr);
	~ColorSVCanvas();

	bool setHue(int hue);
	bool setSaturationValue(int saturation, int value);
	bool setSaturationValue(QPoint saturationValue);

	QColor color();
	QRect availabilityRect();
	int margin();

signals:
	void sig_colorChanged(const QColor &color);

protected:
	void paintEvent(QPaintEvent* ev) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent* ev) Q_DECL_OVERRIDE;
	bool eventFilter(QObject* obj, QEvent* ev) Q_DECL_OVERRIDE;

private:
	QPoint valueFromPos(QPoint& pos);
	QPoint posFromValue(QPoint& val);

private:
	int m_margin;
	int m_radius;
	int m_saturationMax;
	int m_valueMax;

	int m_hue;
	QPoint m_pos;
};

class ColorChecker : public QWidget
{
	Q_OBJECT

public:
	ColorChecker(QWidget *parent = nullptr);
	~ColorChecker();

protected:
	void paintEvent(QPaintEvent* ev) Q_DECL_OVERRIDE;

private:
	int m_checkerSize;
};

class ColorAlphaBar : public QWidget
{
	Q_OBJECT

public:
	ColorAlphaBar(QWidget *parent = nullptr);
	~ColorAlphaBar();

	void setColor(QColor color);
	QColor color();

	void setValue(int alpha);

signals:
	void sig_colorChanged(const QColor &color);

protected:
	void resizeEvent(QResizeEvent* ev) Q_DECL_OVERRIDE;

private:
	ColorChecker* m_checker;
	QSlider* m_slider;
	QColor m_color;

	int m_grooveHeight;
};

class ColorWorkbench : public QDialog
{
	Q_OBJECT

public:
	ColorWorkbench(QWidget *parent = nullptr);
	~ColorWorkbench();

	void setColor(QColor color);

signals:
	void sig_colorChanged(const QColor &color);

private:
	QColor colorFromStr(QString str);

private slots:
	void slot_colorDisplay(const QColor &color);
	void slot_colorEdit(const QString &text);

private:
	ColorSVCanvas* m_canvas;
	ColorHueBar* m_hsvBar;
	ColorAlphaBar* m_alphaSlider;
	QLineEdit* m_lineEdit;
	QPushButton* m_clearBtn;
	QPushButton* m_confirmBtn;
};

class ColorPicker : public QLabel
{
	Q_OBJECT

public:
	ColorPicker(QWidget *parent = nullptr);
	~ColorPicker();
	void setColor(const QColor &color);
	QColor getColor();

protected:
	void resizeEvent(QResizeEvent* ev) Q_DECL_OVERRIDE;

private slots:
	void slot_showPopup();
	void slot_colorChanged(const QColor &color);

private:
	QPushButton* m_button;
	ColorChecker* m_checker;
	ColorWorkbench* m_popup;

	QColor m_curColor;
	QColor m_oriColor;
};



