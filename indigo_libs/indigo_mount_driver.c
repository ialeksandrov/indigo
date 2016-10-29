//  Copyright (c) 2016 CloudMakers, s. r. o.
//  All rights reserved.
//
//	You can use this software under the terms of 'INDIGO Astronomy
//  open-source license' (see LICENSE.md).
//
//	THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
//	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//	ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
//	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//  version history
//  2.0 Build 0 - PoC by Peter Polakovic <peter.polakovic@cloudmakers.eu>

/** INDIGO Mount Driver base
 \file indigo_mount_driver.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "indigo_mount_driver.h"

indigo_result indigo_mount_device_attach(indigo_device *device, indigo_version version) {
	assert(device != NULL);
	assert(device != NULL);
	if (MOUNT_DEVICE_CONTEXT == NULL) {
		device->device_context = malloc(sizeof(indigo_mount_device_context));
		assert(device->device_context);
		memset(device->device_context, 0, sizeof(indigo_mount_device_context));
	}
	if (MOUNT_DEVICE_CONTEXT != NULL) {
		if (indigo_device_attach(device, version, INDIGO_INTERFACE_MOUNT) == INDIGO_OK) {
			// -------------------------------------------------------------------------------- MOUNT_PARK
			MOUNT_PARK_PROPERTY = indigo_init_switch_property(NULL, device->name, "MOUNT_PARK", MOUNT_MAIN_GROUP, "Park", INDIGO_IDLE_STATE, INDIGO_RW_PERM, INDIGO_ONE_OF_MANY_RULE, 2);
			if (MOUNT_PARK_PROPERTY == NULL)
				return INDIGO_FAILED;
			indigo_init_switch_item(MOUNT_PARK_PARKED_ITEM, "PARKED", "Mount parked", true);
			indigo_init_switch_item(MOUNT_PARK_UNPARKED_ITEM, "UNPARKED", "Mount unparked", false);
			// --------------------------------------------------------------------------------
			return INDIGO_OK;
		}
	}
	return INDIGO_FAILED;
}

indigo_result indigo_mount_device_enumerate_properties(indigo_device *device, indigo_client *client, indigo_property *property) {
	assert(device != NULL);
	assert(device->device_context != NULL);
	indigo_result result = INDIGO_OK;
	if ((result = indigo_device_enumerate_properties(device, client, property)) == INDIGO_OK) {
		if (CONNECTION_CONNECTED_ITEM->sw.value) {
			if (indigo_property_match(MOUNT_PARK_PROPERTY, property))
				indigo_define_property(device, MOUNT_PARK_PROPERTY, NULL);
		}
	}
	return result;
}

indigo_result indigo_mount_device_change_property(indigo_device *device, indigo_client *client, indigo_property *property) {
	assert(device != NULL);
	assert(device->device_context != NULL);
	assert(property != NULL);
	if (indigo_property_match(CONNECTION_PROPERTY, property)) {
		// -------------------------------------------------------------------------------- CONNECTION
		if (CONNECTION_CONNECTED_ITEM->sw.value) {
			indigo_define_property(device, MOUNT_PARK_PROPERTY, NULL);
		} else {
			indigo_delete_property(device, MOUNT_PARK_PROPERTY, NULL);
		}
		// --------------------------------------------------------------------------------
	}
	return indigo_device_change_property(device, client, property);
}

indigo_result indigo_mount_device_detach(indigo_device *device) {
	assert(device != NULL);
	if (CONNECTION_CONNECTED_ITEM->sw.value) {
		indigo_delete_property(device, MOUNT_PARK_PROPERTY, NULL);
	}
	free(MOUNT_PARK_PROPERTY);
	return indigo_device_detach(device);
}
