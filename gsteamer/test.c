#include <gst/gst.h>  

int main(int argc, char *argv[])
{  
	GstElement *pipeline;  
	GstBus *bus;  
	GstMessage *msg;  

	/* Initialize GStreamer */  
	gst_init (&argc, &argv);  

	/* Build the pipeline */  
	pipeline = gst_parse_launch ("playbin uri=rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov", NULL);  
	printf("pip = %d\n",pipeline);
	/* Start playing */  
	gst_element_set_state (pipeline, GST_STATE_PLAYING);  

	/* Wait until error or EOS */  
	bus = gst_element_get_bus (pipeline);  
	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);  

	/* Free resources */  
	if (msg != NULL)  
		gst_message_unref (msg);  
	gst_object_unref (bus);  
	gst_element_set_state (pipeline, GST_STATE_NULL);  
	gst_object_unref (pipeline);  
	return 0;  
}  
