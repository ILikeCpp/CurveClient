#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T08:36:20
#
#-------------------------------------------------

QT       += core gui sql widgets printsupport opengl svg serialport network
CONFIG += qaxcontainer  #release


DEFINES += VER_WINDOWS
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    readdatathread.cpp \
    xlineedit.cpp \
    SerialPort/posix_qextserialport.cpp \
    SerialPort/qextserialbase.cpp \
    SerialPort/qextserialport.cpp \
    SerialPort/SerialPort.cpp \
    SerialPort/SerialPortDialog.cpp \
    SerialPort/win_qextserialport.cpp \
    datacom.cpp \
    aboutdialog.cpp \
    setdialog.cpp \
    xcombobox.cpp \
    accountitem.cpp \
    listwidget.cpp \
    commoninterface.cpp \
    qwt-6.1.3/qwt_abstract_legend.cpp \
    qwt-6.1.3/qwt_abstract_scale.cpp \
    qwt-6.1.3/qwt_abstract_scale_draw.cpp \
    qwt-6.1.3/qwt_abstract_slider.cpp \
    qwt-6.1.3/qwt_analog_clock.cpp \
    qwt-6.1.3/qwt_arrow_button.cpp \
    qwt-6.1.3/qwt_clipper.cpp \
    qwt-6.1.3/qwt_color_map.cpp \
    qwt-6.1.3/qwt_column_symbol.cpp \
    qwt-6.1.3/qwt_compass.cpp \
    qwt-6.1.3/qwt_compass_rose.cpp \
    qwt-6.1.3/qwt_counter.cpp \
    qwt-6.1.3/qwt_curve_fitter.cpp \
    qwt-6.1.3/qwt_date.cpp \
    qwt-6.1.3/qwt_date_scale_draw.cpp \
    qwt-6.1.3/qwt_date_scale_engine.cpp \
    qwt-6.1.3/qwt_dial.cpp \
    qwt-6.1.3/qwt_dial_needle.cpp \
    qwt-6.1.3/qwt_dyngrid_layout.cpp \
    qwt-6.1.3/qwt_event_pattern.cpp \
    qwt-6.1.3/qwt_graphic.cpp \
    qwt-6.1.3/qwt_interval.cpp \
    qwt-6.1.3/qwt_interval_symbol.cpp \
    qwt-6.1.3/qwt_knob.cpp \
    qwt-6.1.3/qwt_legend.cpp \
    qwt-6.1.3/qwt_legend_data.cpp \
    qwt-6.1.3/qwt_legend_label.cpp \
    qwt-6.1.3/qwt_magnifier.cpp \
    qwt-6.1.3/qwt_math.cpp \
    qwt-6.1.3/qwt_matrix_raster_data.cpp \
    qwt-6.1.3/qwt_null_paintdevice.cpp \
    qwt-6.1.3/qwt_painter.cpp \
    qwt-6.1.3/qwt_painter_command.cpp \
    qwt-6.1.3/qwt_panner.cpp \
    qwt-6.1.3/qwt_picker.cpp \
    qwt-6.1.3/qwt_picker_machine.cpp \
    qwt-6.1.3/qwt_pixel_matrix.cpp \
    qwt-6.1.3/qwt_plot.cpp \
    qwt-6.1.3/qwt_plot_abstract_barchart.cpp \
    qwt-6.1.3/qwt_plot_axis.cpp \
    qwt-6.1.3/qwt_plot_barchart.cpp \
    qwt-6.1.3/qwt_plot_canvas.cpp \
    qwt-6.1.3/qwt_plot_curve.cpp \
    qwt-6.1.3/qwt_plot_dict.cpp \
    qwt-6.1.3/qwt_plot_directpainter.cpp \
    qwt-6.1.3/qwt_plot_glcanvas.cpp \
    qwt-6.1.3/qwt_plot_grid.cpp \
    qwt-6.1.3/qwt_plot_histogram.cpp \
    qwt-6.1.3/qwt_plot_intervalcurve.cpp \
    qwt-6.1.3/qwt_plot_item.cpp \
    qwt-6.1.3/qwt_plot_layout.cpp \
    qwt-6.1.3/qwt_plot_legenditem.cpp \
    qwt-6.1.3/qwt_plot_magnifier.cpp \
    qwt-6.1.3/qwt_plot_marker.cpp \
    qwt-6.1.3/qwt_plot_multi_barchart.cpp \
    qwt-6.1.3/qwt_plot_panner.cpp \
    qwt-6.1.3/qwt_plot_picker.cpp \
    qwt-6.1.3/qwt_plot_rasteritem.cpp \
    qwt-6.1.3/qwt_plot_renderer.cpp \
    qwt-6.1.3/qwt_plot_rescaler.cpp \
    qwt-6.1.3/qwt_plot_scaleitem.cpp \
    qwt-6.1.3/qwt_plot_seriesitem.cpp \
    qwt-6.1.3/qwt_plot_shapeitem.cpp \
    qwt-6.1.3/qwt_plot_spectrocurve.cpp \
    qwt-6.1.3/qwt_plot_spectrogram.cpp \
    qwt-6.1.3/qwt_plot_svgitem.cpp \
    qwt-6.1.3/qwt_plot_textlabel.cpp \
    qwt-6.1.3/qwt_plot_tradingcurve.cpp \
    qwt-6.1.3/qwt_plot_xml.cpp \
    qwt-6.1.3/qwt_plot_zoneitem.cpp \
    qwt-6.1.3/qwt_plot_zoomer.cpp \
    qwt-6.1.3/qwt_point_3d.cpp \
    qwt-6.1.3/qwt_point_data.cpp \
    qwt-6.1.3/qwt_point_mapper.cpp \
    qwt-6.1.3/qwt_point_polar.cpp \
    qwt-6.1.3/qwt_raster_data.cpp \
    qwt-6.1.3/qwt_round_scale_draw.cpp \
    qwt-6.1.3/qwt_sampling_thread.cpp \
    qwt-6.1.3/qwt_scale_div.cpp \
    qwt-6.1.3/qwt_scale_draw.cpp \
    qwt-6.1.3/qwt_scale_engine.cpp \
    qwt-6.1.3/qwt_scale_map.cpp \
    qwt-6.1.3/qwt_scale_widget.cpp \
    qwt-6.1.3/qwt_series_data.cpp \
    qwt-6.1.3/qwt_slider.cpp \
    qwt-6.1.3/qwt_spline.cpp \
    qwt-6.1.3/qwt_symbol.cpp \
    qwt-6.1.3/qwt_system_clock.cpp \
    qwt-6.1.3/qwt_text.cpp \
    qwt-6.1.3/qwt_text_engine.cpp \
    qwt-6.1.3/qwt_text_label.cpp \
    qwt-6.1.3/qwt_thermo.cpp \
    qwt-6.1.3/qwt_transform.cpp \
    qwt-6.1.3/qwt_wheel.cpp \
    qwt-6.1.3/qwt_widget_overlay.cpp \
    CustomPlot/qcustomplot.cpp


HEADERS  += mainwindow.h \
    readdatathread.h \
    xlineedit.h \
    SerialPort/posix_qextserialport.h \
    SerialPort/qextserialbase.h \
    SerialPort/qextserialport.h \
    SerialPort/SerialPort.h \
    SerialPort/SerialPortDialog.h \
    SerialPort/win_qextserialport.h \
    datacom.h \
    packet_def.h \
    aboutdialog.h \
    setdialog.h \
    xcombobox.h \
    accountitem.h \
    listwidget.h \
    commoninterface.h \
    qwt-6.1.3/qwt.h \
    qwt-6.1.3/qwt_abstract_legend.h \
    qwt-6.1.3/qwt_abstract_scale.h \
    qwt-6.1.3/qwt_abstract_scale_draw.h \
    qwt-6.1.3/qwt_abstract_slider.h \
    qwt-6.1.3/qwt_analog_clock.h \
    qwt-6.1.3/qwt_arrow_button.h \
    qwt-6.1.3/qwt_clipper.h \
    qwt-6.1.3/qwt_color_map.h \
    qwt-6.1.3/qwt_column_symbol.h \
    qwt-6.1.3/qwt_compass.h \
    qwt-6.1.3/qwt_compass_rose.h \
    qwt-6.1.3/qwt_compat.h \
    qwt-6.1.3/qwt_counter.h \
    qwt-6.1.3/qwt_curve_fitter.h \
    qwt-6.1.3/qwt_date.h \
    qwt-6.1.3/qwt_date_scale_draw.h \
    qwt-6.1.3/qwt_date_scale_engine.h \
    qwt-6.1.3/qwt_dial.h \
    qwt-6.1.3/qwt_dial_needle.h \
    qwt-6.1.3/qwt_dyngrid_layout.h \
    qwt-6.1.3/qwt_event_pattern.h \
    qwt-6.1.3/qwt_global.h \
    qwt-6.1.3/qwt_graphic.h \
    qwt-6.1.3/qwt_interval.h \
    qwt-6.1.3/qwt_interval_symbol.h \
    qwt-6.1.3/qwt_knob.h \
    qwt-6.1.3/qwt_legend.h \
    qwt-6.1.3/qwt_legend_data.h \
    qwt-6.1.3/qwt_legend_label.h \
    qwt-6.1.3/qwt_magnifier.h \
    qwt-6.1.3/qwt_math.h \
    qwt-6.1.3/qwt_matrix_raster_data.h \
    qwt-6.1.3/qwt_null_paintdevice.h \
    qwt-6.1.3/qwt_painter.h \
    qwt-6.1.3/qwt_painter_command.h \
    qwt-6.1.3/qwt_panner.h \
    qwt-6.1.3/qwt_picker.h \
    qwt-6.1.3/qwt_picker_machine.h \
    qwt-6.1.3/qwt_pixel_matrix.h \
    qwt-6.1.3/qwt_plot.h \
    qwt-6.1.3/qwt_plot_abstract_barchart.h \
    qwt-6.1.3/qwt_plot_barchart.h \
    qwt-6.1.3/qwt_plot_canvas.h \
    qwt-6.1.3/qwt_plot_curve.h \
    qwt-6.1.3/qwt_plot_dict.h \
    qwt-6.1.3/qwt_plot_directpainter.h \
    qwt-6.1.3/qwt_plot_glcanvas.h \
    qwt-6.1.3/qwt_plot_grid.h \
    qwt-6.1.3/qwt_plot_histogram.h \
    qwt-6.1.3/qwt_plot_intervalcurve.h \
    qwt-6.1.3/qwt_plot_item.h \
    qwt-6.1.3/qwt_plot_layout.h \
    qwt-6.1.3/qwt_plot_legenditem.h \
    qwt-6.1.3/qwt_plot_magnifier.h \
    qwt-6.1.3/qwt_plot_marker.h \
    qwt-6.1.3/qwt_plot_multi_barchart.h \
    qwt-6.1.3/qwt_plot_panner.h \
    qwt-6.1.3/qwt_plot_picker.h \
    qwt-6.1.3/qwt_plot_rasteritem.h \
    qwt-6.1.3/qwt_plot_renderer.h \
    qwt-6.1.3/qwt_plot_rescaler.h \
    qwt-6.1.3/qwt_plot_scaleitem.h \
    qwt-6.1.3/qwt_plot_seriesitem.h \
    qwt-6.1.3/qwt_plot_shapeitem.h \
    qwt-6.1.3/qwt_plot_spectrocurve.h \
    qwt-6.1.3/qwt_plot_spectrogram.h \
    qwt-6.1.3/qwt_plot_svgitem.h \
    qwt-6.1.3/qwt_plot_textlabel.h \
    qwt-6.1.3/qwt_plot_tradingcurve.h \
    qwt-6.1.3/qwt_plot_zoneitem.h \
    qwt-6.1.3/qwt_plot_zoomer.h \
    qwt-6.1.3/qwt_point_3d.h \
    qwt-6.1.3/qwt_point_data.h \
    qwt-6.1.3/qwt_point_mapper.h \
    qwt-6.1.3/qwt_point_polar.h \
    qwt-6.1.3/qwt_raster_data.h \
    qwt-6.1.3/qwt_round_scale_draw.h \
    qwt-6.1.3/qwt_samples.h \
    qwt-6.1.3/qwt_sampling_thread.h \
    qwt-6.1.3/qwt_scale_div.h \
    qwt-6.1.3/qwt_scale_draw.h \
    qwt-6.1.3/qwt_scale_engine.h \
    qwt-6.1.3/qwt_scale_map.h \
    qwt-6.1.3/qwt_scale_widget.h \
    qwt-6.1.3/qwt_series_data.h \
    qwt-6.1.3/qwt_series_store.h \
    qwt-6.1.3/qwt_slider.h \
    qwt-6.1.3/qwt_spline.h \
    qwt-6.1.3/qwt_symbol.h \
    qwt-6.1.3/qwt_system_clock.h \
    qwt-6.1.3/qwt_text.h \
    qwt-6.1.3/qwt_text_engine.h \
    qwt-6.1.3/qwt_text_label.h \
    qwt-6.1.3/qwt_thermo.h \
    qwt-6.1.3/qwt_transform.h \
    qwt-6.1.3/qwt_wheel.h \
    qwt-6.1.3/qwt_widget_overlay.h \
    CustomPlot/qcustomplot.h

FORMS    += \
    mainwindowbase.ui \
    SerialPort/SerialPortDialog.ui \
    aboutdialog.ui \
    setdialog.ui \
    helpdialog.ui

OTHER_FILES += \
    images/about.png \
    images/Action.png \
    images/add.png \
    images/apply.png \
    images/backup.png \
    images/bottom.png \
    images/browse.png \
    images/cancel.png \
    images/clear.png \
    images/contexthelp.png \
    images/copy.png \
    images/down.png \
    images/edit.png \
    images/enter.png \
    images/general.png \
    images/grub_entries.png \
    images/grub_settings.png \
    images/language.png \
    images/lock.png \
    images/ok.png \
    images/open.png \
    images/path.png \
    images/preview.png \
    images/PreviewAction.png \
    images/qgrubeditor.png \
    images/qt.png \
    images/quit.png \
    images/reload.png \
    images/remove.png \
    images/restore.png \
    images/restore_default.png \
    images/saveas.png \
    images/scan.png \
    images/settings.png \
    images/splash.png \
    images/top.png \
    images/up.png \
    images/view.png

RESOURCES += \
    images/images.qrc


RC_FILE = \
   logo.rc

DISTFILES += \
    ccdlog.png

# Win:
WIN_OUT_ROOT = ../../bin
WIN_LIB_ROOT = ../../libs

win32 {
    DESTDIR = $$WIN_OUT_ROOT

    LIBS += \
        -L$$WIN_LIB_ROOT \
        -lQtXlsx
}

INCLUDEPATH += ../QtXlsx

CONFIG += c++11

