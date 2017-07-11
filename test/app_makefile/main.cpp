/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <mono.h>

#include "app_controller.h"


__attribute__((weak)) int main()
{
    AppController app;
    
    mono::IApplicationContext::Instance->setMonoApplication(&app);
    
    app.enterRunLoop();
	
	return 0;
}

/* [] END OF FILE */
