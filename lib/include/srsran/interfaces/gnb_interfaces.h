/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSRAN_GNB_INTERFACES_H
#define SRSRAN_GNB_INTERFACES_H

#include "srsran/srsran.h"

#include "srsran/common/interfaces_common.h"
#include "srsran/common/security.h"
#include "srsran/interfaces/pdcp_interface_types.h"
#include "srsran/interfaces/rlc_interface_types.h"
#include "srsran/interfaces/rrc_interface_types.h"
#include "srsran/interfaces/sched_interface.h"

namespace srsenb {

/*****************************
 *      MAC INTERFACES
 ****************************/
class mac_interface_rrc_nr
{
public:
  // Provides cell configuration including SIB periodicity, etc.
  virtual int cell_cfg(srsenb::sched_interface::cell_cfg_t* cell_cfg) = 0;
};

class mac_interface_rlc_nr
{
public:
  virtual int rlc_buffer_state(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue) = 0;
};

/*****************************
 *      RLC INTERFACES
 ****************************/
class rlc_interface_mac_nr
{
public:
  /* MAC calls RLC to get RLC segment of nof_bytes length.
   * Segmentation happens in this function. RLC PDU is stored in payload. */
  virtual int  read_pdu(uint16_t rnti, uint32_t lcid, uint8_t* payload, uint32_t nof_bytes) = 0;
  virtual void read_pdu_pcch(uint8_t* payload, uint32_t buffer_size)                        = 0;

  /* MAC calls RLC to push an RLC PDU. This function is called from an independent MAC thread.
   * PDU gets placed into the buffer and higher layer thread gets notified. */
  virtual void write_pdu(uint16_t rnti, uint32_t lcid, uint8_t* payload, uint32_t nof_bytes) = 0;
};
class rlc_interface_pdcp_nr
{
public:
  /* PDCP calls RLC to push an RLC SDU. SDU gets placed into the RLC buffer and MAC pulls
   * RLC PDUs according to TB size. */
  virtual void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu) = 0;
  virtual bool rb_is_um(uint16_t rnti, uint32_t lcid)                                    = 0;
  virtual bool sdu_queue_is_full(uint16_t rnti, uint32_t lcid)                           = 0;
};
class rlc_interface_rrc_nr
{
public:
  virtual void clear_buffer(uint16_t rnti)                                               = 0;
  virtual void add_user(uint16_t rnti)                                                   = 0;
  virtual void rem_user(uint16_t rnti)                                                   = 0;
  virtual void add_bearer(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg)       = 0;
  virtual void add_bearer_mrb(uint16_t rnti, uint32_t lcid)                              = 0;
  virtual void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu) = 0;
};

/*****************************
 *      PDCP INTERFACES
 ****************************/
class pdcp_interface_rlc_nr
{
public:
  /* RLC calls PDCP to push a PDCP PDU. */
  virtual void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)               = 0;
  virtual void notify_delivery(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t& pdcp_sns) = 0;
  virtual void notify_failure(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t& pdcp_sns)  = 0;
};

class pdcp_interface_rrc_nr
{
public:
  virtual void reset(uint16_t rnti)                                                                = 0;
  virtual void add_user(uint16_t rnti)                                                             = 0;
  virtual void rem_user(uint16_t rnti)                                                             = 0;
  virtual void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)           = 0;
  virtual void add_bearer(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg)                = 0;
  virtual void config_security(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg) = 0;
  virtual void enable_integrity(uint16_t rnti, uint32_t lcid)                                      = 0;
  virtual void enable_encryption(uint16_t rnti, uint32_t lcid)                                     = 0;
};

class pdcp_interface_sdap_nr
{
public:
  virtual void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu) = 0;
};

/*****************************
 *      SDAP INTERFACES
 ****************************/
class sdap_interface_pdcp_nr
{
public:
  virtual void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu) = 0;
};
class sdap_interface_gtpu_nr
{
public:
  virtual void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu) = 0;
};

/*****************************
 *      GTPU INTERFACES
 ****************************/
class gtpu_interface_rrc_nr
{
public:
};

class gtpu_interface_sdap_nr
{
public:
  virtual void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu) = 0;
};

/*****************************
 *      RRC INTERFACES
 ****************************/
class rrc_interface_phy_nr
{};
class rrc_interface_mac_nr
{
public:
  // Provides MIB packed message
  virtual int read_pdu_bcch_bch(const uint32_t tti, srsran::unique_byte_buffer_t& buffer)   = 0;
  virtual int read_pdu_bcch_dlsch(uint32_t sib_index, srsran::unique_byte_buffer_t& buffer) = 0;
};
class rrc_interface_rlc_nr
{
public:
  virtual void        read_pdu_pcch(uint8_t* payload, uint32_t payload_size)                    = 0;
  virtual void        max_retx_attempted(uint16_t rnti)                                         = 0;
  virtual void        protocol_failure(uint16_t rnti)                                           = 0;
  virtual void        write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu) = 0;
  virtual const char* get_rb_name(uint32_t lcid)                                                = 0;
};
class rrc_interface_pdcp_nr
{
public:
  virtual void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu) = 0;
  virtual void notify_pdcp_integrity_error(uint16_t rnti, uint32_t lcid)                 = 0;
};

class phy_interface_stack_nr
{
public:
  const static int MAX_DL_GRANTS = 4;

  typedef struct {
    // TODO: include NR related fields
  } dl_sched_grant_t;

  typedef struct {
    bool mib_present;
  } bch_sched_t;

  typedef struct {
    uint32_t         tti;
    uint32_t         nof_grants;
    dl_sched_grant_t pdsch[MAX_DL_GRANTS];
    int              beam_id;
  } dl_config_request_t;

  typedef struct {
    bch_sched_t pbch;
    uint16_t    length;
    uint16_t    index;               // index indicated in dl_config
    uint8_t*    data[SRSRAN_MAX_TB]; // always a pointer in our case
  } tx_request_pdu_t;

  typedef struct {
    uint32_t         tti;
    uint32_t         tb_len;
    uint32_t         nof_pdus;
    tx_request_pdu_t pdus[MAX_DL_GRANTS];
  } tx_request_t;

  virtual int dl_config_request(const dl_config_request_t& request) = 0;
  virtual int tx_request(const tx_request_t& request)               = 0;
};

class stack_interface_mac
{
public:
  ///< MAC calls stack to inform about new PDUs having arrived to be further processes in stack thread
  virtual void process_pdus() = 0;
};

class mac_interface_phy_nr
{
public:
  const static int MAX_GRANTS = 64;

  /**
   * DL grant structure per UE
   */
  struct dl_sched_grant_t {
    srsran_dci_dl_nr_t      dci                          = {};
    uint8_t*                data[SRSRAN_MAX_TB]          = {};
    srsran_softbuffer_tx_t* softbuffer_tx[SRSRAN_MAX_TB] = {};
  };

  /**
   * DL Scheduling result per cell/carrier
   */
  struct dl_sched_t {
    dl_sched_grant_t pdsch[MAX_GRANTS]; //< DL Grants
    uint32_t         nof_grants;        //< Number of DL grants
  };

  /**
   * List of DL scheduling results, one entry per cell/carrier
   */
  typedef std::vector<dl_sched_t> dl_sched_list_t;

  /**
   * UL grant structure per UE
   */
  struct ul_sched_grant_t {
    srsran_dci_ul_nr_t      dci           = {};
    uint8_t*                data          = nullptr;
    srsran_softbuffer_rx_t* softbuffer_rx = nullptr;
  };

  /**
   * UL Scheduling result per cell/carrier
   */
  struct ul_sched_t {
    ul_sched_grant_t pusch[MAX_GRANTS]; //< UL Grants
    uint32_t         nof_grants;        //< Number of UL grants
  };

  /**
   * List of UL scheduling results, one entry per cell/carrier
   */
  typedef std::vector<ul_sched_t> ul_sched_list_t;

  virtual int get_dl_sched(uint32_t tti, dl_sched_list_t& dl_sched_res) = 0;
  virtual int get_ul_sched(uint32_t tti, ul_sched_list_t& ul_sched_res) = 0;
};

class stack_interface_phy_nr : public mac_interface_phy_nr, public srsran::stack_interface_phy_nr
{
public:
  struct rx_data_ind_t {
    uint32_t                     tti;
    uint16_t                     rnti;
    srsran::unique_byte_buffer_t tb;
  };

  virtual int sf_indication(const uint32_t tti)        = 0;
  virtual int rx_data_indication(rx_data_ind_t& grant) = 0;
};

} // namespace srsenb

#endif // SRSRAN_GNB_INTERFACES_H
