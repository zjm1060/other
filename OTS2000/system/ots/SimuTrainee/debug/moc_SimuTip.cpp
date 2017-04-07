/****************************************************************************
** Meta object code from reading C++ file 'SimuTip.h'
**
** Created: Thu Jan 22 17:27:27 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/SimuTip.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SimuTip.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimuTipThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      25,   15,   14,   14, 0x05,
      55,   50,   14,   14, 0x25,
      76,   15,   14,   14, 0x05,
     101,   50,   14,   14, 0x25,

       0        // eod
};

static const char qt_meta_stringdata_SimuTipThread[] = {
    "SimuTipThread\0\0step,mode\0"
    "sigStepFinished(int,int)\0step\0"
    "sigStepFinished(int)\0sigStepOvertime(int,int)\0"
    "sigStepOvertime(int)\0"
};

const QMetaObject SimuTipThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SimuTipThread,
      qt_meta_data_SimuTipThread, 0 }
};

const QMetaObject *SimuTipThread::metaObject() const
{
    return &staticMetaObject;
}

void *SimuTipThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimuTipThread))
        return static_cast<void*>(const_cast< SimuTipThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SimuTipThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigStepFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: sigStepFinished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: sigStepOvertime((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: sigStepOvertime((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SimuTipThread::sigStepFinished(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, 1, _a);
}

// SIGNAL 2
void SimuTipThread::sigStepOvertime(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, 3, _a);
}
static const uint qt_meta_data_CSetSimuTip[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      31,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   12,   12,   12, 0x08,
      67,   12,   12,   12, 0x08,
      82,   12,   12,   12, 0x08,
     107,   12,   12,   12, 0x08,
     128,   12,   12,   12, 0x08,
     142,   12,   12,   12, 0x08,
     161,   12,   12,   12, 0x08,
     175,   12,   12,   12, 0x08,
     196,   12,   12,   12, 0x08,
     220,   12,   12,   12, 0x08,
     239,   12,   12,   12, 0x08,
     258,   12,   12,   12, 0x08,
     275,   12,   12,   12, 0x08,
     303,   12,   12,   12, 0x08,
     321,   12,   12,   12, 0x08,
     342,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CSetSimuTip[] = {
    "CSetSimuTip\0\0sigUpdateAttrib()\0"
    "sig_taskaccepted()\0setText(QString)\0"
    "openSetColor()\0openSetBackgroundColor()\0"
    "openSetCurTipColor()\0openSetFont()\0"
    "changeOpacity(int)\0setFontShow()\0"
    "tipColorChanged(int)\0curTipColorChanged(int)\0"
    "setDialogMode(int)\0setDialogSize(int)\0"
    "setAutoSize(int)\0backgroundColorChanged(int)\0"
    "openSimuTip(bool)\0buttonEnterClicked()\0"
    "buttonApplyClicked()\0"
};

const QMetaObject CSetSimuTip::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSetSimuTip,
      qt_meta_data_CSetSimuTip, 0 }
};

const QMetaObject *CSetSimuTip::metaObject() const
{
    return &staticMetaObject;
}

void *CSetSimuTip::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSetSimuTip))
        return static_cast<void*>(const_cast< CSetSimuTip*>(this));
    if (!strcmp(_clname, "Ui_CSetSimuTip"))
        return static_cast< Ui_CSetSimuTip*>(const_cast< CSetSimuTip*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSetSimuTip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigUpdateAttrib(); break;
        case 1: sig_taskaccepted(); break;
        case 2: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: openSetColor(); break;
        case 4: openSetBackgroundColor(); break;
        case 5: openSetCurTipColor(); break;
        case 6: openSetFont(); break;
        case 7: changeOpacity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setFontShow(); break;
        case 9: tipColorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: curTipColorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: setDialogMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: setDialogSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: setAutoSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: backgroundColorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: openSimuTip((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: buttonEnterClicked(); break;
        case 17: buttonApplyClicked(); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void CSetSimuTip::sigUpdateAttrib()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CSetSimuTip::sig_taskaccepted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CSimuTipDlg[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      30,   12,   12,   12, 0x0a,
      47,   12,   12,   12, 0x0a,
      62,   12,   12,   12, 0x0a,
      80,   12,   12,   12, 0x0a,
     100,   12,   12,   12, 0x0a,
     127,   12,   12,   12, 0x0a,
     142,   12,   12,   12, 0x0a,
     158,   12,   12,   12, 0x0a,
     181,   12,   12,   12, 0x0a,
     202,   12,   12,   12, 0x0a,
     217,   12,   12,   12, 0x08,
     224,   12,   12,   12, 0x08,
     239,  233,   12,   12, 0x08,
     263,   12,   12,   12, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_CSimuTipDlg[] = {
    "CSimuTipDlg\0\0openSetSimuTip()\0"
    "setText(QString)\0setSize(QSize)\0"
    "setAutoSize(bool)\0setTipColor(QColor)\0"
    "setBackgroundColor(QColor)\0setFont(QFont)\0"
    "setOpacity(int)\0setCurTipColor(QColor)\0"
    "setTransparent(bool)\0updateAttrib()\0"
    "lock()\0unLock()\0,mode\0setCurStepShow(int,int)\0"
    "setCurStepShow(int)\0"
};

const QMetaObject CSimuTipDlg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSimuTipDlg,
      qt_meta_data_CSimuTipDlg, 0 }
};

const QMetaObject *CSimuTipDlg::metaObject() const
{
    return &staticMetaObject;
}

void *CSimuTipDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSimuTipDlg))
        return static_cast<void*>(const_cast< CSimuTipDlg*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSimuTipDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openSetSimuTip(); break;
        case 1: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: setSize((*reinterpret_cast< QSize(*)>(_a[1]))); break;
        case 3: setAutoSize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: setTipColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 5: setBackgroundColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 6: setFont((*reinterpret_cast< QFont(*)>(_a[1]))); break;
        case 7: setOpacity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setCurTipColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 9: setTransparent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: updateAttrib(); break;
        case 11: lock(); break;
        case 12: unLock(); break;
        case 13: setCurStepShow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: setCurStepShow((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
