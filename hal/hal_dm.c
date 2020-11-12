/******************************************************************************
 *
 * Copyright(c) 2014 Realtek Corporation. All rights reserved.
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

#include <drv_types.h>
#include <hal_data.h>

// A mapping from HalData to ODM.
ODM_BOARD_TYPE_E rtl8188fu_boardType(u8 InterfaceSel)
{
    ODM_BOARD_TYPE_E        board	= ODM_BOARD_DEFAULT;

#ifdef CONFIG_PCI_HCI
	INTERFACE_SELECT_PCIE   pcie 	= (INTERFACE_SELECT_PCIE)InterfaceSel;
	switch (pcie) 
	{
        case INTF_SEL0_SOLO_MINICARD:       
            board |= ODM_BOARD_MINICARD;
            break;
        case INTF_SEL1_BT_COMBO_MINICARD:   
            board |= ODM_BOARD_BT;
			board |= ODM_BOARD_MINICARD;
            break;
        default:
            board = ODM_BOARD_DEFAULT;
            break;
	}                                

#elif defined(CONFIG_USB_HCI)
	INTERFACE_SELECT_USB    usb 	= (INTERFACE_SELECT_USB)InterfaceSel;
	switch (usb) 
	{
	    case INTF_SEL1_USB_High_Power:      
	        board |= ODM_BOARD_EXT_LNA;
	        board |= ODM_BOARD_EXT_PA;			
	        break;
	    case INTF_SEL2_MINICARD:            
	        board |= ODM_BOARD_MINICARD;
	        break;
	    case INTF_SEL4_USB_Combo:           
	        board |= ODM_BOARD_BT;
	        break;
	    case INTF_SEL5_USB_Combo_MF:        
	        board |= ODM_BOARD_BT;
	        break;
	    case INTF_SEL0_USB: 			
	    case INTF_SEL3_USB_Solo:            			
	    default:
	        board = ODM_BOARD_DEFAULT;
	        break;
	}
	
#endif	
	//DBG_871X("===> rtl8188fu_boardType(): (pHalData->InterfaceSel, pDM_Odm->BoardType) = (%d, %d)\n", InterfaceSel, board);

	return board;
}

void rtl8188fu_Init_ODM_ComInfo(_adapter *adapter)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	PHAL_DATA_TYPE	pHalData = GET_HAL_DATA(adapter);
	PDM_ODM_T		pDM_Odm = &(pHalData->odmpriv);
	struct mlme_ext_priv	*pmlmeext = &adapter->mlmeextpriv;
	struct mlme_priv	*pmlmepriv = &adapter->mlmepriv;
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(adapter);
	int i;

	rtl8188fu__rtw_memset(pDM_Odm,0,sizeof(*pDM_Odm));

	pDM_Odm->Adapter = adapter;

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_PLATFORM, ODM_CE);

	rtl8188fu_rtw_odm_init_ic_type(adapter);

		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_INTERFACE, rtw_get_intf_type(adapter));

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_MP_TEST_CHIP, IS_NORMAL_CHIP(pHalData->VersionID));

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_PATCH_ID, pHalData->CustomerID);

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_BWIFI_TEST, adapter->registrypriv.wifi_spec);


	if (pHalData->rf_type == RF_1T1R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_1T1R);
	else if (pHalData->rf_type == RF_1T2R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_1T2R);
	else if (pHalData->rf_type == RF_2T2R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_2T2R);
	else if (pHalData->rf_type == RF_2T2R_GREEN)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_2T2R_GREEN);
	else if (pHalData->rf_type == RF_2T3R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_2T3R);
	else if (pHalData->rf_type == RF_2T4R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_2T4R);
	else if (pHalData->rf_type == RF_3T3R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_3T3R);
	else if (pHalData->rf_type == RF_3T4R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_3T4R);
	else if (pHalData->rf_type == RF_4T4R)
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_4T4R);
	else
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_TYPE, ODM_XTXR);
	

{
	//1 ======= BoardType: ODM_CMNINFO_BOARD_TYPE =======
	u8 odm_board_type = ODM_BOARD_DEFAULT;

	if (pHalData->ExternalLNA_2G != 0) {
		odm_board_type |= ODM_BOARD_EXT_LNA;
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_EXT_LNA, 1);
	}
	if (pHalData->ExternalLNA_5G != 0) {
		odm_board_type |= ODM_BOARD_EXT_LNA_5G;
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_5G_EXT_LNA, 1);
	}
	if (pHalData->ExternalPA_2G != 0) {
		odm_board_type |= ODM_BOARD_EXT_PA;
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_EXT_PA, 1);
	}
	if (pHalData->ExternalPA_5G != 0) {
		odm_board_type |= ODM_BOARD_EXT_PA_5G;
		rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_5G_EXT_PA, 1);
	}
	if (pHalData->EEPROMBluetoothCoexist)
		odm_board_type |= ODM_BOARD_BT;	

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_BOARD_TYPE, odm_board_type);
	//1 ============== End of BoardType ==============
}

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_DOMAIN_CODE_2G, pHalData->Regulation2_4G);
	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_DOMAIN_CODE_5G, pHalData->Regulation5G);

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_GPA, pHalData->TypeGPA);
	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_APA, pHalData->TypeAPA);
	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_GLNA, pHalData->TypeGLNA);
	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_ALNA, pHalData->TypeALNA);

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RFE_TYPE, pHalData->RFEType);

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_EXT_TRSW, 0);

	rtl8188fu_ODM_CmnInfoInit(pDM_Odm, ODM_CMNINFO_RF_ANTENNA_TYPE, pHalData->TRxAntDivType);

	/* Pointer reference */
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_TX_UNI, &(dvobj->traffic_stat.tx_bytes));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_RX_UNI, &(dvobj->traffic_stat.rx_bytes));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_WM_MODE, &(pmlmeext->cur_wireless_mode));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_BAND, &(pHalData->CurrentBandType));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_FORCED_RATE, &(pHalData->ForcedDataRate));

	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_SEC_CHNL_OFFSET, &(pHalData->nCur40MhzPrimeSC));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_SEC_MODE, &(adapter->securitypriv.dot11PrivacyAlgrthm));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_BW, &(pHalData->CurrentChannelBW));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_CHNL, &( pHalData->CurrentChannel));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_NET_CLOSED, &(adapter->net_closed));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_FORCED_IGI_LB, &(pHalData->u1ForcedIgiLb));

	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_SCAN, &(pmlmepriv->bScanInProcess));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_POWER_SAVING, &(pwrctl->bpower_saving));
	/*Add by Yuchen for phydm beamforming*/
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_TX_TP, &(dvobj->traffic_stat.cur_tx_tp));
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_RX_TP, &(dvobj->traffic_stat.cur_rx_tp));
#ifdef CONFIG_USB_HCI
	rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_HUBUSBMODE, &(dvobj->usb_speed));
#endif
	for(i=0; i<ODM_ASSOCIATE_ENTRY_NUM; i++)
		rtl8188fu_ODM_CmnInfoPtrArrayHook(pDM_Odm, ODM_CMNINFO_STA_STATUS, i, NULL);

	/* TODO */
	//rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_BT_OPERATION, _FALSE);
	//rtl8188fu_ODM_CmnInfoHook(pDM_Odm, ODM_CMNINFO_BT_DISABLE_EDCA, _FALSE);
}

