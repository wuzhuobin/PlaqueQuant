/****************************************************************************
** Meta object code from reading C++ file 'ModuleWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../codes/Module/GUI/ModuleWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModuleWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ModuleWidget_t {
    QByteArrayData data[13];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModuleWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModuleWidget_t qt_meta_stringdata_ModuleWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ModuleWidget"
QT_MOC_LITERAL(1, 13, 8), // "finished"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "busy"
QT_MOC_LITERAL(4, 28, 20), // "slotSegmentationView"
QT_MOC_LITERAL(5, 49, 17), // "slotChangeLayerNo"
QT_MOC_LITERAL(6, 67, 13), // "slotSelectROI"
QT_MOC_LITERAL(7, 81, 17), // "slotChangeOpacity"
QT_MOC_LITERAL(8, 99, 17), // "NextBtnChangeText"
QT_MOC_LITERAL(9, 117, 8), // "NextPage"
QT_MOC_LITERAL(10, 126, 8), // "BackPage"
QT_MOC_LITERAL(11, 135, 12), // "SetBrushSize"
QT_MOC_LITERAL(12, 148, 18) // "slotReportGetInput"

    },
    "ModuleWidget\0finished\0\0busy\0"
    "slotSegmentationView\0slotChangeLayerNo\0"
    "slotSelectROI\0slotChangeOpacity\0"
    "NextBtnChangeText\0NextPage\0BackPage\0"
    "SetBrushSize\0slotReportGetInput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModuleWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    2,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   75,    2, 0x0a /* Public */,
       5,    0,   76,    2, 0x0a /* Public */,
       6,    0,   77,    2, 0x0a /* Public */,
       7,    0,   78,    2, 0x0a /* Public */,
       8,    0,   79,    2, 0x0a /* Public */,
       9,    0,   80,    2, 0x0a /* Public */,
      10,    0,   81,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x0a /* Public */,
      12,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModuleWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ModuleWidget *_t = static_cast<ModuleWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->busy((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->slotSegmentationView(); break;
        case 3: _t->slotChangeLayerNo(); break;
        case 4: _t->slotSelectROI(); break;
        case 5: _t->slotChangeOpacity(); break;
        case 6: _t->NextBtnChangeText(); break;
        case 7: _t->NextPage(); break;
        case 8: _t->BackPage(); break;
        case 9: _t->SetBrushSize(); break;
        case 10: _t->slotReportGetInput(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ModuleWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ModuleWidget::finished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ModuleWidget::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ModuleWidget::busy)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ModuleWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ModuleWidget.data,
      qt_meta_data_ModuleWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ModuleWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModuleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ModuleWidget.stringdata0))
        return static_cast<void*>(const_cast< ModuleWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ModuleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ModuleWidget::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ModuleWidget::busy(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
