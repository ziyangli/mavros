/**
 * @brief Manualsetpoint plugin
 * @file manual_pub.cpp
 * @author Ziyang Li <ziyang.d.lee@gmail.com>
 *
 * @example manual_pub.cpp
 * @addtogroup plugin
 * @{
 */
/*
 * Copyright 2014 Ziyang Li
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include <mavros/ManualSetpoint.h>

namespace mavplugin {

/**
 * @brief manual_setpoint plugin.
 *
 */
class ManualSetpointPlugin : public MavRosPlugin {
public:
    ManualSetpointPlugin() :
        nh("~"),
        uas(nullptr) { };

    void initialize(UAS &uas_) {
        uas = &uas_;
        pubSP = nh.advertise<mavros::ManualSetpoint>("manual", 10);
    };

	const message_map get_rx_handlers() {
		return {
			MESSAGE_HANDLER(MAVLINK_MSG_ID_MANUAL_CONTROL, &ManualSetpointPlugin::handle_manual_control),
		};
	}

private:

    ros::NodeHandle nh;
    UAS *uas;

    ros::Publisher pubSP;

	void handle_manual_control(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {
        mavlink_manual_control_t man_ctrl;
        mavlink_msg_manual_control_decode(msg, &man_ctrl);

        mavros::ManualSetpointPtr man_sp_msg = boost::make_shared<mavros::ManualSetpoint>();

        man_sp_msg->header.stamp = ros::Time::now();
        man_sp_msg->x = man_ctrl.x;
        man_sp_msg->y = man_ctrl.y;
        man_sp_msg->z = man_ctrl.z;
        man_sp_msg->r = man_ctrl.r;
        man_sp_msg->buttons = man_ctrl.buttons;
        pubSP.publish(man_sp_msg);
	}

};

}; // namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::ManualSetpointPlugin, mavplugin::MavRosPlugin)

