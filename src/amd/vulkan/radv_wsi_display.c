/*
 * Copyright © 2017 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "radv_private.h"
#include "radv_cs.h"
#include "util/disk_cache.h"
#include "util/strtod.h"
#include "vk_util.h"
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <amdgpu.h>
#include <amdgpu_drm.h>
#include "winsys/amdgpu/radv_amdgpu_winsys_public.h"
#include "ac_llvm_util.h"
#include "vk_format.h"
#include "sid.h"
#include "util/debug.h"
#include "wsi_common_display.h"

#define MM_PER_PIXEL     (1.0/96.0 * 25.4)

VkResult
radv_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physical_device,
                                           uint32_t *property_count,
                                           VkDisplayPropertiesKHR *properties)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_display_get_physical_device_display_properties(
		physical_device,
		&pdevice->wsi_device,
		property_count,
		properties);
}

VkResult
radv_GetPhysicalDeviceDisplayPlanePropertiesKHR(
	VkPhysicalDevice physical_device,
	uint32_t *property_count,
	VkDisplayPlanePropertiesKHR *properties)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_display_get_physical_device_display_plane_properties(
		physical_device,
		&pdevice->wsi_device,
		property_count,
		properties);
}

VkResult
radv_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physical_device,
                                         uint32_t plane_index,
                                         uint32_t *display_count,
                                         VkDisplayKHR *displays)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_display_get_display_plane_supported_displays(
		physical_device,
		&pdevice->wsi_device,
		plane_index,
		display_count,
		displays);
}


VkResult
radv_GetDisplayModePropertiesKHR(VkPhysicalDevice physical_device,
                                 VkDisplayKHR display,
                                 uint32_t *property_count,
                                 VkDisplayModePropertiesKHR *properties)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_display_get_display_mode_properties(physical_device,
						       &pdevice->wsi_device,
						       display,
						       property_count,
						       properties);
}

VkResult
radv_CreateDisplayModeKHR(VkPhysicalDevice physical_device,
                          VkDisplayKHR display,
                          const VkDisplayModeCreateInfoKHR *create_info,
                          const VkAllocationCallbacks *allocator,
                          VkDisplayModeKHR *mode)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_display_create_display_mode(physical_device,
					       &pdevice->wsi_device,
					       display,
					       create_info,
					       allocator,
					       mode);
}

VkResult
radv_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physical_device,
                                    VkDisplayModeKHR mode_khr,
                                    uint32_t plane_index,
                                    VkDisplayPlaneCapabilitiesKHR *capabilities)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_get_display_plane_capabilities(physical_device,
						  &pdevice->wsi_device,
						  mode_khr,
						  plane_index,
						  capabilities);
}

VkResult
radv_CreateDisplayPlaneSurfaceKHR(
	VkInstance _instance,
	const VkDisplaySurfaceCreateInfoKHR *create_info,
	const VkAllocationCallbacks *allocator,
	VkSurfaceKHR *surface)
{
	RADV_FROM_HANDLE(radv_instance, instance, _instance);
	const VkAllocationCallbacks *alloc;

	if (allocator)
		alloc = allocator;
	else
		alloc = &instance->alloc;

	return wsi_create_display_surface(_instance, alloc,
					  create_info, surface);
}

VkResult
radv_ReleaseDisplayEXT(VkPhysicalDevice physical_device,
		       VkDisplayKHR     display)
{
	RADV_FROM_HANDLE(radv_physical_device, pdevice, physical_device);

	return wsi_release_display(physical_device,
				   &pdevice->wsi_device,
				   display);
}