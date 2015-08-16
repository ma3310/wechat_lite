<?php

require_once realpath(dirname(__FILE__)) . '/include/common.inc';

/**
 * @param $segment string
 */
function service_account_server ($segment){

  if (isset($segment)){

    $service_accounts = variable_get(VAR_WECHAT_LITE_SERVICE_ACCOUNTS, array());

    if (isset($service_accounts[$segment])){
      //


      $account = $service_accounts[$segment];

      echo $segment;
      return;
    }

  }

  // Only
  header(t(RET_HTTP_SERVICE_NOT_CONFIG), TRUE, 503);

}