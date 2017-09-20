/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

/******************************************************************************
 * File:        ue.h
 * Description: Top-level UE class. Creates and links all
 *              layers and helpers.
 *****************************************************************************/

#ifndef UE_H
#define UE_H

#include <stdarg.h>
#include <string>
#include <pthread.h>

#include "ue_base.h"
#include "srslte/radio/radio_multi.h"
#include "phy/phy.h"
#include "mac/mac.h"
#include "srslte/upper/rlc.h"
#include "srslte/upper/pdcp.h"
#include "upper/rrc.h"
#include "upper/nas.h"
#include "upper/gw.h"
#include "upper/usim.h"

#include "srslte/common/buffer_pool.h"
#include "srslte/interfaces/ue_interfaces.h"
#include "srslte/common/logger_file.h"
#include "srslte/common/log_filter.h"

#include "ue_metrics_interface.h"

namespace srsue {

//#define LOG_STDOUT

/*******************************************************************************
  Main UE class
*******************************************************************************/

class ue
    :public ue_base
{
public:
  ue();

  bool init(all_args_t *args_);
  void stop();
  bool is_attached();
  void start_plot();

  static void rf_msg(srslte_rf_error_t error);
  void handle_rf_msg(srslte_rf_error_t error);

  // UE metrics interface
  bool get_metrics(ue_metrics_t &m);

  void pregenerate_signals(bool enable);


private:
  virtual ~ue();

  srslte::radio_multi radio;
  srsue::phy         phy;
  srsue::mac         mac;
  srslte::mac_pcap   mac_pcap;
  srslte::rlc        rlc;
  srslte::pdcp       pdcp;
  srsue::rrc         rrc;
  srsue::nas         nas;
  srsue::gw          gw;
  srsue::usim        usim;

#ifdef LOG_STDOUT
    srslte::logger_stdout logger;
#else
    srslte::logger_file logger;
#endif
  // rf_log is on ue_base
  srslte::log_filter  phy_log;
  srslte::log_filter  mac_log;
  srslte::log_filter  rlc_log;
  srslte::log_filter  pdcp_log;
  srslte::log_filter  rrc_log;
  srslte::log_filter  nas_log;
  srslte::log_filter  gw_log;
  srslte::log_filter  usim_log;

  srslte::byte_buffer_pool *pool;

  all_args_t       *args;
  bool              started;
  rf_metrics_t     rf_metrics;
  
  bool check_srslte_version();
};

} // namespace srsue

#endif // UE_H
  
