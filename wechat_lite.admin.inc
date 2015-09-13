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

  $service_accounts = variable_get(VAR_WECHAT_LITE_SERVICE_ACCOUNTS, array());

  $ret = FALSE;
  if (isset($service_accounts[$segment])) {
    $ret = TRUE;
  }

  return $ret;
}

/**
 * Determines if segment exists as a enterprise account.
 *
 * @param $segment string
 *   The ID of the text format to check.
 *
 * @return bool
 *   TRUE if segment exists as a service account, FALSE otherwise.
 *
 */
function enterprise_accounts_segment_exists($segment) {

  $accounts = variable_get(VAR_WECHAT_LITE_ENTERPRISE_ACCOUNTS, array());

  $ret = FALSE;
  if (isset($accounts[$segment])) {
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
function wechat_lite_service_account_form($form, &$form_state, $account_segment = NULL) {

  // if $account is empty, need initial $account, treat this situation as Account Adding
  if (!isset($account_segment)) {
    drupal_set_title(t('Add WeChat Service Account'));
    $account = (object) array(
      'name' => '',
      'segment' => '',
      'appid' => '',
      'appsecret' => '',
      'token' => '',
    );
  }
  else {
    $service_accounts = variable_get(VAR_WECHAT_LITE_SERVICE_ACCOUNTS, array());
    $account = (object)$service_accounts[$account_segment];
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

  $form['appid'] = array(
    '#type' => 'textfield',
    '#title' => t('AppID'),
    '#size' => 20,
    '#default_value' => $account->appid,
    '#required' => TRUE,
  );

  $form['appsecret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#description' => t(<<<'TAG'
Get this information from privilege management sub-page of settings page.<br>
Application has to be put in non-SystemAdmin group first, then use Secret of that group.
TAG
    ),
    '#default_value' => $account->appsecret,
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

function wechat_lite_enterprise_account_form($form, &$form_state, $account_segment = NULL) {

  // if $account is empty, need initial $account, treat this situation as Account Adding
  if (!isset($account_segment)) {
    drupal_set_title(t('Add WeChat Enterprise Account'));
    $account = (object) array(
      'name' => '',
      'segment' => '',
      'corpid' => '',
      'token' => '',
      'encodingAesKey' => '',
    );
  }
  else {
    $accounts = variable_get(VAR_WECHAT_LITE_ENTERPRISE_ACCOUNTS, array());
    $account = (object)$accounts[$account_segment];
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Name'),
    '#description' => t('Give a name to identify each WeChat Enterprise Account.'),
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
      'exists' => 'enterprise_accounts_segment_exists',
    ),
    '#disabled' => !empty($account->segment),
  );

  $form['corpid'] = array(
    '#type' => 'textfield',
    '#title' => t('CorpID'),
    '#description' => t(<<<'TAG'
Get this information from company setting page.
TAG
    ),
    '#size' => 20,
    '#default_value' => $account->corpid,
    '#required' => TRUE,
  );

  $form['token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#default_value' => $account->token,
    '#required' => TRUE,
  );

  $form['encodingAesKey'] = array(
    '#type' => 'textfield',
    '#title' => t('encodingAesKey'),
    '#default_value' => $account->encodingAesKey,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Account'),
  );

  return $form;

}

function wechat_lite_enterprise_account_form_submit($form, &$form_state) {

  $account = array(
    'name' => $form['name']['#value'],
    'segment' => $form['segment']['#value'],
    'corpid' => $form['corpid']['#value'],
    'token' => $form['token']['#value'],
    'encodingAesKey' => $form['encodingAesKey']['#value'],
    'weight' => 0,
  );

  $accounts = variable_get(VAR_WECHAT_LITE_ENTERPRISE_ACCOUNTS, array());
  $accounts[$account['segment']] = $account;
  variable_set(VAR_WECHAT_LITE_ENTERPRISE_ACCOUNTS, $accounts);

  $form_state['redirect'] = 'admin/config/system/wechat_lite/enterprise_accounts';

}

/**
 * Form builder.
 * Configure Service Accounts related options.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function wechat_lite_service_accounts_settings($form, &$form_state) {


  $form['#tree'] = TRUE;

  $service_accounts = variable_get(VAR_WECHAT_LITE_SERVICE_ACCOUNTS, array());

  $form['service_accounts']['#theme'] = "wechat_lite_service_accounts_settings";

  // Put array as raw table, need theme to render
  foreach ($service_accounts as $segment => $account) {
    $form['service_accounts'][$segment]['name'] = array('#markup' => drupal_placeholder($account['name']));
    $form['service_accounts'][$segment]['segment'] = array('#markup' => url('wechat_lite/service_account/'.$segment.'/server', array('absolute'=>TRUE)));
    $form['service_accounts'][$segment]['weight'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => $account['weight'],
      '#attributes' => array('class' => array('wechat-accounts-weight')),
    );

    $form['service_accounts'][$segment]['configure'] = array(
      '#type' => 'link',
      '#title' => t('configure'),
      '#href' => 'admin/config/system/wechat_lite/service_accounts/' . $segment);
  }

  $form['#submit'][] = 'wechat_lite_service_accounts_settings_submit';

  return system_settings_form($form);

}

/**
 * Form builder.
 * Configure Enterprise Accounts.
 * 1. List all configured enterprise accounts
 *
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function wechat_lite_enterprise_accounts_settings($form, &$form_state) {

  $form['#tree'] = TRUE;



  return system_settings_form($form);

}

function theme_wechat_lite_service_accounts_settings ($vars){

  $element = $vars['element'];

  drupal_add_tabledrag('sample_table', 'order', 'sibling', 'wechat-accounts-weight');

  $header = array(
    'name' => t('Account Name'),
    'segment' => t('WeChat Callback URL'),
    'weight' => t('Weight'),
    'configure' => t('Operations'),
  );

  $rows = array();

  foreach (element_children($element) as $key) {
    $row = array();
    $row['data'] = array();
    foreach ($header as $fieldname => $title) {
      $row['data'][] = drupal_render($element[$key][$fieldname]);
      $row['class'] = array('draggable');
    }
    $rows[] = $row;
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'sample_table'),
  ));
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
function wechat_lite_service_accounts_settings_validate($form, &$form_state) {
  /*if($form['name']['#value'] == '')
      form_set_error('name', 'Account name can not be empty.');
  elseif($form['appid']['value'] == '')
      form_set_error('appid', 'Appid can not be empty.');
  elseif($form['appsecret']['value'] == '')
      form_set_error('appid', 'Appsecret can not be empty.');
  elseif($form['token']['value'] == '')
      form_set_error('token', 'Token can not be empty.');*/
}

/**
 *
 *
 * @param $form
 * @param $form_state
 *
 */
function wechat_lite_service_accounts_settings_submit($form, &$form_state) {

}