#include  <glib.h>
#include  <gst/gst.h>
#include  <gst/video/videooverlay.h>

#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QObject>
/***************   STUBS ***************/
#define DEFAULT_VIDEOSINK "autovideosink"
/*************************************/

/***************   COMPILE FLAG ********/
#define RUN_GST_PARSE_LAUNCH 0
/*************************************/

#define IF_NULL_THEN_RET(var, name) if(!(var)) { g_printerr ("%s is NULL. Ret -1.\n", (name) );  return -1;}

typedef struct _CustomData {
    GstElement  *pipeline;
    GstElement  *source;
    GstElement  *demuxer;
    GstElement  *aqueue;
    GstElement  *vqueue;
    GstElement  *adecoder;
    GstElement  *vdecoder;
    GstElement  *aconvert;
    GstElement  *vconvert;
    GstElement  *asink;
    GstElement  *vsink;

    GstElement  *volume;
    QFrame*movieFrame;
    QSlider *timeSlider;
    QSlider *volumeSlider;
    QLabel  *curTimeLabel;
    QLabel  *durTimeLabel;

    GstState state;                 /* Current state of the pipeline */
    gint64 duration;              /* Duration of the clip, in nanoseconds */

}CustomData ;

static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data) ;
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data) ;

static gboolean refresh_position(CustomData*);
static void on_pad_added (GstElement *element, GstPad *pad, CustomData *data );

int main(int argc, char *argv[]){
    CustomData data;

    GstBus *bus;
    GstStateChangeReturn ret;

    //data.duration = GST_CLOCK_TIME_NONE;
    gst_init (&argc, &argv);    /* Initialize GStreamer */
    QApplication app(argc, argv);


    /* Create our own GLib Main Context and make it the default one */
    GMainContext* context = g_main_context_new ();
    g_main_context_push_thread_default(context);

    GST_DEBUG ("Creating pipeline");
    /* Create the empty pipeline */
    data.pipeline= gst_pipeline_new ("my_pipeline");

    /* Create elements */
    data.source = gst_element_factory_make ("filesrc", "source");
    data.demuxer=gst_element_factory_make ("matroskademux", "demuxer");

    data.aqueue= gst_element_factory_make ("queue", "aqueue");
    data.adecoder = gst_element_factory_make("vorbisdec", NULL);
    data.aconvert = gst_element_factory_make ("audioconvert", "convert");
    data.volume = gst_element_factory_make("volume", NULL);
    data.asink = gst_element_factory_make ("autoaudiosink", "sink");

    data.vqueue = gst_element_factory_make ("queue", "vqueue");
    data.vdecoder = gst_element_factory_make("vp8dec", "vdecoder");
    data.vconvert = gst_element_factory_make("videoconvert", NULL);
    data.vsink = gst_element_factory_make("ximagesink", NULL);

    if(!(data.pipeline)) { g_printerr ("pipeline is NULL. Ret -1.\n");  return -1;}
    if(!(data.source))  { g_printerr ("source is NULL. Ret -1.\n");  return -1;}
    if(!(data.demuxer)) { g_printerr ("demuxer is NULL. Ret -1.\n");  return -1;}
    if(!(data.aqueue)) { g_printerr ("aqueue is NULL. Ret -1.\n");  return -1;}
    if(!(data.aconvert)) { g_printerr ("aconvert is NULL. Ret -1.\n");  return -1;}
    if(!(data.asink)) { g_printerr ("asink is NULL. Ret -1.\n");  return -1;}
    if(!(data.vqueue)) { g_printerr ("vqueue is NULL. Ret -1.\n");  return -1;}
    if(!(data.vconvert)) { g_printerr ("vconvert is NULL. Ret -1.\n");  return -1;}
    if(!(data.vdecoder)) { g_printerr ("vdecoder is NULL. Ret -1.\n");  return -1;}
    if(!(data.vsink)) { g_printerr ("vsink is NULL. Ret -1.\n");  return -1;}
    if(!(data.volume)) { g_printerr ("volume is NULL. Ret -1.\n");  return -1;}

    gst_bin_add_many (
                GST_BIN (data.pipeline),
                data.source, data.demuxer,
                data.vqueue, data.vdecoder, data.vconvert, data.vsink,
                data.aqueue , data.adecoder, data.aconvert, data.volume, data.asink,
                NULL);

    if(!gst_element_link (data.source, data.demuxer))
        g_print("[main] ERROR: gst_element_link (source, demuxer) ");

    if(!gst_element_link_many (data.vqueue, data.vdecoder, data.vconvert, data.vsink, NULL) )
        g_print("[main] ERROR: gst_element_link_many (data.vdecoder, data.vconvert, data.vsink, NULL) ");

    if(!gst_element_link_many (data.aqueue, data.adecoder, data.aconvert,data.volume, data.asink, NULL) )
        g_print("[main] ERROR: gst_element_link_many (data.demuxer, data.adecoder, data.aconvert,data.avolume, data.asink, NULL)  ");

    /* prepare the ui */
    MainWindow window(NULL, data.pipeline, data.volume);
    window.setWindowTitle("Qt&GStreamer Simple Player");
    window.show();
    WId xwinid = window.get_movieFrame_winId();

    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (data.vsink), xwinid); //???????????
    
    /* set custom data */
    data.duration = GST_CLOCK_TIME_NONE;
    data.timeSlider = window.get_timeSlider_ptr();
    data.curTimeLabel = window.get_curTimeLabel();
    data.durTimeLabel = window.get_durTimeLabel();

    bus = gst_element_get_bus (data.pipeline);
    gst_bus_add_signal_watch (bus);

    g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, &data);
    g_signal_connect (G_OBJECT (bus), "message::eos", (GCallback)eos_cb, &data);
    g_signal_connect (G_OBJECT (bus), "message::state-changed", (GCallback)state_changed_cb, &data);
    g_signal_connect (data.demuxer, "pad-added", G_CALLBACK (on_pad_added),  &data);
    g_timeout_add_seconds (1, (GSourceFunc)refresh_position, &data);

	char video_file[10];
	if (argv[1] != NULL)
	{
		strcpy(video_file,argv[1]);
		g_printerr ("video_file = %s\n",video_file);
	}else{
		g_printerr ("ERROR information: please input video_file");
 		return -1;
	}
    
	/* Set the source to play */
    g_object_set (G_OBJECT (data.source), "location", video_file, NULL);

    printf(" yq	 --> set path \n");
    /* Start playing */
    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        gst_element_set_state (data.pipeline, GST_STATE_NULL);
        gst_object_unref (data.pipeline);
        g_printerr ("ERROR information: gst_element_set_state %d\n",ret);
        return -1;
    }
    return app.exec();
}

static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    GError *err;
    gchar *debug_info;

    /* Print error details on the screen */
    gst_message_parse_error (msg, &err, &debug_info);
    g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");

    g_clear_error (&err);
    g_free (debug_info);

    /* Set the pipeline to READY (which stops playback) */
    gst_element_set_state (data->pipeline, GST_STATE_READY);
    (void)bus;
}

static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    g_print ("End-Of-Stream reached.\n");
    gst_element_set_state (data->pipeline, GST_STATE_READY);
    (void)bus;
    (void)msg;
}

static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data)
{
    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->pipeline)) {
        GstState old_state, new_state, pending_state;
        gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
        g_print ("Pipeline state changed from %s to %s:\n",  gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
        data->state = new_state;

        /*Refresh the GUI as soon as we reach the PAUSED state */
        if (old_state == GST_STATE_READY && new_state == GST_STATE_PAUSED)
        {
            refresh_position (data);
        }
    }

    (void)bus;
}

static gboolean refresh_position (CustomData *data)
{
    GstFormat fmt = GST_FORMAT_TIME;
    gint64 current = -1;

    /* We do not want to update anything unless we are in the PAUSED or PLAYING states */
    if (data->state < GST_STATE_PAUSED)
        return TRUE;

    /* If we didn't know it yet, query the stream duration */
 #if 1
    if (!GST_CLOCK_TIME_IS_VALID (data->duration))
    {
        if (!gst_element_query_duration (data->pipeline, fmt, &data->duration))
        {
            g_printerr ("Could not query current duration.\n");
        }
        else
        {
            /* Set the range of the slider to the clip duration, in SECONDS */
            int durTime = (int) ((gdouble)data->duration / GST_SECOND);
            data->durTimeLabel->setText(MainWindow::get_time_string(durTime));
            data->timeSlider->setRange(0,  durTime);
        }
    }
 #endif
    if (gst_element_query_position (data->pipeline, fmt, &current))
    {
        /* Set the position of the slider to the current pipeline positoin, in SECONDS */
        int curTime = (int) ((gdouble)current / GST_SECOND);
        data->curTimeLabel->setText(MainWindow::get_time_string(curTime));
        data->timeSlider->setValue(curTime);
    }
    return TRUE;
}

static void on_pad_added (GstElement *element, GstPad *pad, CustomData *data )
{
    gchar *pad_name = NULL;

    g_object_get(pad, "name", &pad_name, NULL);

    if(g_str_has_prefix(pad_name, "video"))
    {
        GstPad *v_sinkpad = NULL;

        v_sinkpad = gst_element_get_static_pad (data->vqueue, "sink");
        gst_pad_link (pad, v_sinkpad);

/*        if(!gst_pad_link (pad, v_sinkpad))
            g_print("%s linked to vdecoder.\n",pad_name);
        else
            g_print("ERROR: gst_pad_link (pad, v_sinkpad), pad_name = %s\n",pad_name);
*/
        gst_object_unref (v_sinkpad);
    }

    if(g_str_has_prefix(pad_name, "audio"))
    {
        GstPad *a_sinkpad;

        a_sinkpad = gst_element_get_static_pad (data->aqueue, "sink");
        if(!gst_pad_link (pad, a_sinkpad))
            g_print("%s linked to adecoder.\n", pad_name);
        else
            g_print("ERROR: gst_pad_link (pad, a_sinkpad), pad_name = %s\n",pad_name);

        gst_object_unref (a_sinkpad);
    }
}


