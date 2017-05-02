// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef __mono__mono__
#define __mono__mono__

#include <stdint.h>

// MARK: Common

#include <async.h>
#include <consoles.h>
#include <date_time.h>
#include <managed_pointer.h>
#include <mn_string.h>
#include <mn_timer.h>
#include <queue.h>
#include <url.h>
#include <scheduled_task.h>
#include <regex.h>

// MARK: Core Mono Runtime

#include <application_context_interface.h>
#include <application_controller_interface.h>
#include <application_run_loop_task_interface.h>

// MARK: Display

#include <display/color.h>
#include <display/display_painter.h>
#include <display/text_render.h>

// MARK: UI

#include <display/ui/abstract_button_view.h>
#include <display/ui/animator.h>
#include <display/ui/background_view.h>
#include <display/ui/button_view.h>
#include <display/ui/console_view.h>
#include <display/ui/graph_view.h>
#include <display/ui/icon_view.h>
#include <display/ui/image_view.h>
#include <display/ui/on_off_button_view.h>
#include <display/ui/progress_bar_view.h>
#include <display/ui/responder_view.h>
#include <display/ui/scene_controller.h>
#include <display/ui/status_indicator_view.h>
#include <display/ui/text_label_view.h>
#include <display/ui/touch_calibrator.h>
#include <display/ui/view.h>

// MARK: Geo

#include <circle.h>
#include <point.h>
#include <rect.h>
#include <size.h>

// MARK: IO

#include <io/file.h>
#include <io/filtered_analog_in.h>
#include <io/hysteresis_trigger.h>
#include <io/mn_digital_out.h>
#include <queue_interrupt.h>

// MARK: Media

#include <media/bmp_image.h>
#include <media/image.h>

// MARK: Power

#include <power_aware_interface.h>
#include <power_management_interface.h>
#include <power_saver.h>

// MARK: Sensors

#include <touch_system_interface.h>
#include <buzzer_interface.h>
#include <temperature_interface.h>
#include <accelerometer_interface.h>

#ifndef EMUNO
#include <mono_battery.h>
#include <http_client.h>
#include <http_post_client.h>
#include <dns_resolver.h>
#else
#include <emuno.h>
#endif

namespace mono {
}

#endif // __mono__mono__
