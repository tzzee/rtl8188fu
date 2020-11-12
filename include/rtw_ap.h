/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTW_AP_H_
#define __RTW_AP_H_


#ifdef CONFIG_AP_MODE

//external function
extern void rtl8188fu_rtw_indicate_sta_assoc_event(_adapter *padapter, struct sta_info *psta);
extern void rtl8188fu_rtw_indicate_sta_disassoc_event(_adapter *padapter, struct sta_info *psta);


void rtl8188fu_init_mlme_ap_info(_adapter *padapter);
void rtl8188fu_free_mlme_ap_info(_adapter *padapter);
//void update_BCNTIM(_adapter *padapter);
void rtl8188fu_rtw_add_bcn_ie(_adapter *padapter, WLAN_BSSID_EX *pnetwork, u8 index, u8 *data, u8 len);
void rtl8188fu_rtw_remove_bcn_ie(_adapter *padapter, WLAN_BSSID_EX *pnetwork, u8 index);
void rtl8188fu__update_beacon(_adapter *padapter, u8 ie_id, u8 *oui, u8 tx, const char *tag);
#define update_beacon(adapter, ie_id, oui, tx) rtl8188fu__update_beacon((adapter), (ie_id), (oui), (tx), __func__)
void rtl8188fu_add_RATid(_adapter *padapter, struct sta_info *psta, u8 rssi_level);
void rtl8188fu_expire_timeout_chk(_adapter *padapter);
void rtl8188fu_update_sta_info_apmode(_adapter *padapter, struct sta_info *psta);
void rtl8188fu_rtw_start_bss_hdl_after_chbw_decided(_adapter *adapter);
void rtl8188fu_start_bss_network(_adapter *padapter, struct createbss_parm *parm);
int rtl8188fu_rtw_check_beacon_data(_adapter *padapter, u8 *pbuf,  int len);
void rtl8188fu_rtw_ap_restore_network(_adapter *padapter);
void rtl8188fu_rtw_set_macaddr_acl(_adapter *padapter, int mode);
int rtl8188fu_rtw_acl_add_sta(_adapter *padapter, u8 *addr);
int rtl8188fu_rtw_acl_remove_sta(_adapter *padapter, u8 *addr);

u8 rtl8188fu_rtw_ap_set_pairwise_key(_adapter *padapter, struct sta_info *psta);
int rtl8188fu_rtw_ap_set_group_key(_adapter *padapter, u8 *key, u8 alg, int keyid);
int rtl8188fu_rtw_ap_set_wep_key(_adapter *padapter, u8 *key, u8 keylen, int keyid, u8 set_tx);

#ifdef CONFIG_NATIVEAP_MLME
void rtl8188fu_associated_clients_update(_adapter *padapter, u8 updated, u32 sta_info_type);
void rtl8188fu_bss_cap_update_on_sta_join(_adapter *padapter, struct sta_info *psta);
u8 rtl8188fu_bss_cap_update_on_sta_leave(_adapter *padapter, struct sta_info *psta);
void rtl8188fu_sta_info_update(_adapter *padapter, struct sta_info *psta);
void rtl8188fu_ap_sta_info_defer_update(_adapter *padapter, struct sta_info *psta);
u8 rtl8188fu_ap_free_sta(_adapter *padapter, struct sta_info *psta, bool active, u16 reason, bool enqueue);
int rtl8188fu_rtw_sta_flush(_adapter *padapter, bool enqueue);
int rtl8188fu_rtw_ap_inform_ch_switch(_adapter *padapter, u8 new_ch, u8 ch_offset);
void rtl8188fu_start_ap_mode(_adapter *padapter);
void rtl8188fu_stop_ap_mode(_adapter *padapter);
#endif

void rtl8188fu_rtw_ap_update_bss_chbw(_adapter *adapter, WLAN_BSSID_EX *bss, u8 ch, u8 bw, u8 offset);
bool rtl8188fu_rtw_ap_chbw_decision(_adapter *adapter, u8 req_ch, u8 req_bw, u8 req_offset, u8 *ch, u8 *bw, u8 *offset);

#ifdef CONFIG_AUTO_AP_MODE
extern void rtw_start_auto_ap(_adapter *adapter);
#endif //CONFIG_AUTO_AP_MODE

#endif //end of CONFIG_AP_MODE

#endif
void rtl8188fu_update_bmc_sta(_adapter *padapter);

void rtl8188fu_rtw_process_ht_action_smps(_adapter *padapter, u8 *ta, u8 ctrl_field);
void rtl8188fu_rtw_process_public_act_bsscoex(_adapter *padapter, u8 *pframe, uint frame_len);

