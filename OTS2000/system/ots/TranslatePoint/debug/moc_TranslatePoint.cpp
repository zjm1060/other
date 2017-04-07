/****************************************************************************
** Meta object code from reading C++ file 'TranslatePoint.h'
**
** Created: Tue Jun 3 14:32:52 2014
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../inc/TranslatePoint.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TranslatePoint.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTranslatePoint[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      33,   26,   16,   16, 0x08,
      82,   16,   16,   16, 0x08,
     103,   97,   16,   16, 0x08,
     119,   16,   16,   16, 0x08,
     131,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CTranslatePoint[] = {
    "CTranslatePoint\0\0isQuit()\0reason\0"
    "iconActivated(QSystemTrayIcon::ActivationReason)\0"
    "setStartGrab()\0state\0oncheckbox(int)\0"
    "checkQuit()\0openPopupMsg()\0"
};

const QMetaObject CTranslatePoint::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CTranslatePoint,
      qt_meta_data_CTranslatePoint, 0 }
};

const QMetaObject *CTranslatePoint::metaObject() const
{
    return &staticMetaObject;
}

void *CTranslatePoint::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTranslatePoint))
        return static_cast<void*>(const_cast< CTranslatePoint*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CTranslatePoint::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: isQuit(); break;
        case 1: iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 2: setStartGrab(); break;
        case 3: oncheckbox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: checkQuit(); break;
        case 5: openPopupMsg(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
