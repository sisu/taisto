/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
<<<<<<< HEAD
** Created: Tue Oct 27 18:21:58 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
=======
** Created: Tue Oct 27 21:35:56 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
>>>>>>> 050d80d042200044edab064e913a14e57b83b970
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Window[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    8,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Window[] = {
    "Window\0\0,\0draw(QList<Object*>&,QList<Object*>&)\0"
};

const QMetaObject Window::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Window,
      qt_meta_data_Window, 0 }
};

const QMetaObject *Window::metaObject() const
{
    return &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Window))
        return static_cast<void*>(const_cast< Window*>(this));
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
<<<<<<< HEAD
        case 0: draw(); break;
        default: ;
=======
        case 0: draw((*reinterpret_cast< QList<Object*>(*)>(_a[1])),(*reinterpret_cast< QList<Object*>(*)>(_a[2]))); break;
>>>>>>> 050d80d042200044edab064e913a14e57b83b970
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
