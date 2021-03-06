/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Code Aurora Forum nor
 *       the names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *
 * Alternatively, provided that this notice is retained in full, this software
 * may be relicensed by the recipient under the terms of the GNU General Public
 * License version 2 ("GPL") and only version 2, in which case the provisions of
 * the GPL apply INSTEAD OF those given above.  If the recipient relicenses the
 * software under the GPL, then the identification text in the MODULE_LICENSE
 * macro must be changed to reflect "GPLv2" instead of "Dual BSD/GPL".  Once a
 * recipient changes the license terms to the GPL, subsequent recipients shall
 * not relicense under alternate licensing terms, including the BSD or dual
 * BSD/GPL terms.  In addition, the following license statement immediately
 * below and between the words START and END shall also then apply when this
 * software is relicensed under the GPL:
 *
 * START
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 and only version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * END
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "msm_fb.h"

#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
#include "mddihosti.h"
#endif

#ifdef CONFIG_MSM_HDMI 
#include "hdmi.h"
#endif

//#define 720P_60HZ	1
static int __init lcdc_prism_init(void)
{
	int ret;
	struct msm_panel_info pinfo;
#ifdef CONFIG_MSM_HDMI 
	boolean isCableConnected = FALSE;
#endif

#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
	ret = msm_fb_detect_client("lcdc_prism_wvga");
	if (ret == -ENODEV)
		return 0;

	if (ret && (mddi_get_client_id() != 0))
		return 0;
#endif
	
#ifdef CONFIG_MSM_HDMI 

#ifdef CONFIG_MSM_HDMI_POWER_OFF_WHEN_LP
    hdmi_shutdown();
#else
    if (0 == hdmi_startup())
    {
#if 0 // C01 产品暂不做处理
	    hdmi_create_proc_file();

	    if(hdmi_get_plug_status())
	    {
	        isCableConnected = TRUE;
	    }

	    if (!isCableConnected)
	    {
	        hdmi_change_dss_drive_strength();
	    }
#else
        hdmi_standby();
#endif
    }
	else // 兼容HDMI芯片未焊接情形
	{
		hdmi_shutdown();
	}
#endif //CONFIG_MSM_HDMI_POWER_SAVING

#ifdef HDMI_DEBUG
    if (isCableConnected)
    {
        printk( KERN_INFO " HYQ cable connected!!\n");
    }
    else
    {
        printk( KERN_INFO " HYQ cable not connected!! \n ");
    }
#endif
	
if (isCableConnected)
{
	pinfo.xres = 1280; 
	pinfo.yres = 720;

	pinfo.type =  LCDC_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.fb_num = 2;

	pinfo.clk_rate = 74250000;
	pinfo.clk_min = 74250000;
	pinfo.clk_max = 74250000;
	pinfo.bl_min = 0;
	pinfo.bl_max = 255;
	pinfo.lcdc.h_back_porch = 220; 
	pinfo.lcdc.h_front_porch = 110;
	pinfo.lcdc.h_pulse_width = 40;
	pinfo.lcdc.v_back_porch = 20;
	pinfo.lcdc.v_front_porch = 5;
	pinfo.lcdc.v_pulse_width = 5;

	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;

	ret = hdmi_device_register(&pinfo);
}

else
{
#endif
	pinfo.xres = 800;
	pinfo.yres = 480;
	pinfo.type = LCDC_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.fb_num = 2;
	pinfo.clk_rate = 38460000;

#if 1  
	pinfo.lcdc.h_back_porch = 46;
	pinfo.lcdc.h_front_porch = 30;
	pinfo.lcdc.h_pulse_width = 24;
	pinfo.lcdc.v_back_porch = 32;
	pinfo.lcdc.v_front_porch = 9;
	pinfo.lcdc.v_pulse_width = 4;
#else
	pinfo.lcdc.h_back_porch = 21;
	pinfo.lcdc.h_front_porch = 81;
	pinfo.lcdc.h_pulse_width = 60;
	pinfo.lcdc.v_back_porch = 18;
	pinfo.lcdc.v_front_porch = 27;
	pinfo.lcdc.v_pulse_width = 2;
#endif
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;

	ret = lcdc_device_register(&pinfo);
#ifdef CONFIG_MSM_HDMI 
}
#endif
	
	if (ret)
		printk(KERN_ERR "%s: failed to register device!\n", __func__);

	return ret;
}

module_init(lcdc_prism_init);
