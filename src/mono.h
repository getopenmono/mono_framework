// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef __mono__mono__
#define __mono__mono__

#include <stdint.h>
#include <managed_pointer.h>

#include <application_context_interface.h>
#include <application_controller_interface.h>
#include <application_run_loop_task_interface.h>
#include <circle.h>
#include <color.h>
#include <consoles.h>
#include <async.h>
#include <display/ui/animator.h>
#include <display/ui/background_view.h>
#include <display/ui/abstract_button_view.h>
#include <display/ui/on_off_button_view.h>
#include <display/ui/button_view.h>
#include <display/ui/console_view.h>
#include <display/ui/graph_view.h>
#include <display/ui/icon_view.h>
#include <display/ui/image_view.h>
#include <display/ui/progress_bar_view.h>
#include <display/ui/responder_view.h>
#include <display/ui/scene_controller.h>
#include <display/ui/status_indicator_view.h>
#include <display/ui/text_label_view.h>
#include <display/ui/touch_calibrator.h>
#include <display/ui/view.h>
#include <display_painter.h>
#include <media/bmp_image.h>
#include <media/image.h>
#include <mn_digital_out.h>
#include <mn_string.h>
#include <mn_timer.h>
#include <mono_battery.h>
#include <point.h>
#include <queue.h>
#include <queue_interrupt.h>
#include <rect.h>
#include <regex.h>
#include <size.h>
#include <text_render.h>
#include <touch_system_interface.h>
#include <power_saver.h>
#include <date_time.h>
#include <scheduled_task.h>

#ifndef EMUNO
#include <http_client.h>
#include <http_post_client.h>
#include <dns_resolver.h>
#else
#include <emuno.h>
#endif

namespace mono {
}

#endif // __mono__mono__
