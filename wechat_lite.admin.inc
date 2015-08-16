<?php

require_once realpath(dirname(__FILE__)) . '/include/common.inc';

/**
 * @file
 * Administration functions for WeChat Lite module.
 */

/**
 * Determines if segment exists as a service account.
 *
 * @param $segment
 *   The ID of the text format to check.
 *
 * @return bool
 *   TRUE if segment exists as a service account, FALSE otherwise.
 *
 */
function service_accounts_segment_exists($segment) {

  $service_accounts = variable_get(WECHAT_LITE_SERVICE_ACCOUNTS, array());

  $ret = FALSE;
  if (isset($service_accounts[$segment])) {
    $ret = TRUE;
  }

  return $ret;
}

/**
 * Form constructor for the WeChat Service Account add/edit form.
 *
 * @param $account
 *   A format object having the properties:
 *   - format: A machine-readable name representing the ID of the text format to
 *     save. If this corresponds to an existing text format, that format will be
 *     updated; otherwise, a new format will be created.
 *   - name: The title of the text format.
 *   - cache: An integer indicating whether the text format is cacheable (1) or
 *     not (0). Defaults to 1.
 *   - status: (optional) An integer indicating whether the text format is
 *     enabled (1) or not (0). Defaults to 1.
 *   - weight: (optional) The weight of the text format, which controls its
 *     placement in text format lists. If omitted, the weight is set to 0.
 *
 * @see wechat_lite_service_account_form_validate()
 * @see wechat_lite_service_account_form_submit()
 * @ingroup forms
 */
function wechat_lite_service_account_form($form, &$form_state, $account = NULL) {

  // if $account is empty, need initial $account, treat current as Account Adding
  if (!isset($account->name)) {
    drupal_set_title(t('Add WeChat Service Account'));
    $account = (object) array(
      'name' => '',
      'segment' => '',
      'app_id' => '',
      'app_secret' => '',
      'token' => '',
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Name'),
    '#description' => t('Give a name to identify each WeChat Service Account.'),
    '#size' => 20,
    '#default_value' => $account->name,
    '#required' => TRUE,
  );

  $form['segment'] = array(
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#default_value' => $account->segment,
    '#maxlength' => 255,
    '#machine_name' => array(
      'exists' => 'service_accounts_segment_exists',
    ),
    '#disabled' => !empty($account->segment),
  );

  $form['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('AppID'),
    '#size' => 20,
    '#default_value' => $account->app_id,
    '#required' => TRUE,
  );

  $form['app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#description' => t('Get this information from privilege management sub-page of settings page.<br>
Application has to be put in non-SystemAdmin group first, then use Secret of that group.'),
    '#default_value' => $account->app_secret,
    '#required' => TRUE,
  );

  $form['token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#default_value' => $account->token,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Account'),
  );

  return $form;

}

function wechat_lite_service_account_form_submit ($form, &$form_state){

  $account = array(
    'name' => $form['name']['#value'],
    'segment' => $form['segment']['#value'],
    'app_id' => $form['app_id']['#value'],
    'app_secret' => $form['app_secret']['#value'],
    'token' => $form['token']['#value'],
    'weight' => 0,
  );

  $service_accounts = variable_get(WECHAT_LITE_SERVICE_ACCOUNTS, array());
  $service_accounts[$account['segment']] = $account;
  variable_set(WECHAT_LITE_SERVICE_ACCOUNTS, $service_accounts);

  $form_state['redirect'] = 'admin/config/system/wechat_lite/service_accounts';

}

/**
 * Form builder.
 * Configure Service Accounts related options.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function wechat_lite_service_accounts_settings() {




  $form['#tree'] = true;

  $service_accounts = variable_get(WECHAT_LITE_SERVICE_ACCOUNTS, array());

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
function wechat_lite_service_accounts_settings_validate ($form, &$form_state){

}

/**
 *
 *
 * @param $form
 * @param $form_state
 *
 */
function wechat_lite_service_accounts_settings_submit ($form, &$form_state){

}

