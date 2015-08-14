<?php
/**
 * @file
 * Administration functions for WeChat Lite module.
 */

/**
 * Form builder.
 * Configure Service Accounts related options.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function wechat_lite_service_accounts_settings() {

  $form['wechat_lite_service_account_content_encryption_method'] = array(
    '#title' => t('Contents Encryption Method'),
    '#description' => t('Select an encryption method which will be used when changing contents between server with WeChat apps.'),
    '#type' => 'radios',
    '#options' => array(
      t('None'),
      t('AES Encoding'),
    ),
    '#default_value' => variable_get('wechat_lite_service_account_content_encryption_method', 0) // Default to None
  );


  $form['#submit'][] = 'wechat_lite_service_accounts_settings_submit';

  return system_settings_form($form);
}

/**
 * Form Validation
 * prevent invalid values are inputted.
 *
 *
 *
 * @param $form
 * @param $form_state
 */
function wechat_lite_service_accounts_settings_validate ($form, $form_state){

}

/**
 *
 *
 * @param $form
 * @param $form_state
 *
 */
function wechat_lite_service_accounts_settings_submit ($form, $form_state){

}