<?php

require_once realpath(dirname(__FILE__)) . '/include/common.inc';
require_once realpath(dirname(__FILE__)) . '/include/WXBizMsgCrypt.php';

/**
 * @param $segment string
 */
function enterprise_account_server($segment) {

  if (isset($segment)&&!empty($segment)) {

    watchdog(
      MODULE_NAME,
      "Enterprise callback-ed: %segment",
      array('%segment' => $segment)
    );

    // Get all configured accounts information
    $enterprise_accounts = variable_get(
      VAR_WECHAT_LITE_ENTERPRISE_ACCOUNTS,
      array()
    );

    if (!isset($enterprise_accounts[$segment])) {
      return MENU_NOT_FOUND;
    }


    // Get $_GET contents to decide next logic
    $query = drupal_get_query_parameters();

    if (isset($enterprise_accounts[$segment])) {

      $wxcpt = new WXBizMsgCrypt(
        $enterprise_accounts[$segment]['token'],
        $enterprise_accounts[$segment]['encodingAesKey'],
        $enterprise_accounts[$segment]['corpid']
      );

      $decodedString = "";

      // <-- Verify Echo String
      if (isset($query['echostr'])) {

        $errCode = $wxcpt->VerifyURL(
          $query['msg_signature'],
          $query['timestamp'],
          $query['nonce'],
          $query['echostr'],
          $decodedString
        );

        if ($errCode == ErrorCode::$OK) {
          // returns echo string when verify callback URL
          echo ($decodedString);

          return MENU_FOUND;
        }
        else {
          watchdog(
            MODULE_NAME,
            "Enterprise callback error: Code=%errCode",
            array('%errCode' => $errCode)
          );

          return;
        }
      }
      // Verify Echo String -->

      // Receiving interaction message
      $postData = file_get_contents("php://input");

      if (!isset($postData) || empty($postData)){
        return;
      }

      watchdog(
        MODULE_NAME,
        "POST Data: %data",
        array('%data' => $postData)
      );

      $errCode = $wxcpt->DecryptMsg(
        $query['msg_signature'],
        $query['timestamp'],
        $query['nonce'],
        $postData,
        $decodedString
      );

      if ($errCode == ErrorCode::$OK) {
        // HTTP Response
        echo "";
        flush();

        watchdog(
          MODULE_NAME,
          "Received message: %data",
          array('%data' => $decodedString)
        );

        $msg = (array)simplexml_load_string($decodedString, 'SimpleXMLElement', LIBXML_NOCDATA);

        $data = array();

        if ($msg ['MsgType'] != 'event'){

          $data['MsgId'] = $msg['MsgId'];
          $data['ToUserName'] = $msg['ToUserName'];
          $data['MsgType'] = $msg['MsgType'];
          $data['AgentID'] = $msg['AgentID'];
          $data['CreateTime'] = $msg['CreateTime'];

          $wrapper = entity_metadata_wrapper('wechat_lite_message', entity_create('wechat_lite_message', $data));

          if ( $msg ['MsgType'] == 'image' ){

            $data['PicUrl'] = $msg['PicUrl'];
            $data['MediaId'] = $msg['MediaId'];

          }
          $wrapper->save();
        }
        // Event could be handled immediately
        else {

        }

        return MENU_FOUND;
      }
      // DecryptMsg failed
      else {
        watchdog(
          MODULE_NAME,
          "Enterprise callback error: Code=%errCode",
          array('%errCode' => $errCode)
        );

        return;
      }

    }


  }
}

function fetch_received_image (){

  // entity_create replaces the procedural steps in the first example of
  // creating a new object $node and setting its 'type' and uid property
  $values = array(
    'type' => 'ge_contact',
    'uid' => 1,
    'status' => 1,
    'comment' => 0,
    'promote' => 0,
  );
  $entity = entity_create('node', $values);

  // The entity is now created, but we have not yet simplified use of it.
  // Now create an entity_metadata_wrapper around the new node entity
  // to make getting and setting values easier
  $ewrapper = entity_metadata_wrapper('node', $entity);

  // Using the wrapper, we do not have to worry about telling Drupal
  // what language we are using. The Entity API handles that for us.
  $ewrapper->title->set('YOUR TITLE');

  // Setting the body is a bit different from other properties or fields
  // because the body can have both its complete value and its
  // summary
  $my_body_content = 'A bunch of text about things that interest me';
  $ewrapper->body->set(array('value' => $my_body_content));
  $ewrapper->body->summary->set('Things that interest me');

  // Setting the value of an entity reference field only requires passing
  // the entity id (e.g., nid) of the entity to which you want to refer
  // The nid 15 here is just an example.
  $ref_nid = 15;

  // Note that the entity id (e.g., nid) must be passed as an integer not a
  // string
//  $ewrapper->field_my_entity_ref->set(intval($ref_nid));

  // Entity API cannot set date field values so the 'old' method must
  // be used
//  $my_date = new DateTime('January 1, 2013');
//  $entity->field_my_date[LANGUAGE_NONE][0] = array(
//    'value' => date_format($my_date, 'Y-m-d'),
//    'timezone' => 'UTC',
//    'timezone_db' => 'UTC',
//  );


  // Fetch received file
  // $file = media_parse_to_file('http://shp.qpic.cn/bizmp/wvpGjbYWWX4lgkUVdqfxzxrjLcAia8XBxhETbn0d4Ms6uwH7bMnSia9w/');
  // $ewrapper->field_business_card->file->set($file);


  // Now just save the wrapper and the entity
  // There is some suggestion that the 'true' argument is necessary to
  // the entity save method to circumvent a bug in Entity API. If there is
  // such a bug, it almost certainly will get fixed, so make sure to check.


  $ewrapper->save();




  return;
}
