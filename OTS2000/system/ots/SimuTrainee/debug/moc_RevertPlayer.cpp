/****************************************************************************
** Meta object code from reading C++ file 'RevertPlayer.h'
**
** Created: Thu Jan 22 17:27:35 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/RevertPlayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RevertPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CRevertPlayer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      34,   14,   14,   14, 0x08,
      53,   14,   14,   14, 0x08,
      74,   14,   14,   14, 0x08,
      87,   14,   14,   14, 0x08,
     103,   14,   14,   14, 0x08,
     126,  120,   14,   14, 0x08,
     154,   14,   14,   14, 0x08,
     170,   14,   14,   14, 0x08,
     193,  120,   14,   14, 0x08,
     224,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CRevertPlayer[] = {
    "CRevertPlayer\0\0isRevertListShow()\0"
    "setSpeedLabel(int)\0startOrPauseRevert()\0"
    "stopRevert()\0forwardRevert()\0"
    "backwardRevert()\0index\0"
    "revertSelected(QModelIndex)\0updateDisplay()\0"
    "setRevertProgress(int)\0"
    "PopupRevertDetail(QModelIndex)\0"
    "setLanguage(int)\0"
};

const QMetaObject CRevertPlayer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CRevertPlayer,
      qt_meta_data_CRevertPlayer, 0 }
};

const QMetaObject *CRevertPlayer::metaObject() const
{
    return &staticMetaObject;
}

void *CRevertPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CRevertPlayer))
        return static_cast<void*>(const_cast< CRevertPlayer*>(this));
    if (!strcmp(_clname, "Ui_RevertPlayer"))
        return static_cast< Ui_RevertPlayer*>(const_cast< CRevertPlayer*>(this));
    return QWidget::qt_metacast(_clname);
}

int CRevertPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: isRevertListShow(); break;
        case 1: setSpeedLabel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: startOrPauseRevert(); break;
        case 3: stopRevert(); break;
        case 4: forwardRevert(); break;
        case 5: backwardRevert(); break;
        case 6: revertSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: updateDisplay(); break;
        case 8: setRevertProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: PopupRevertDetail((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
