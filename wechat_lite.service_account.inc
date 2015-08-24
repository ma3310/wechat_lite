<?php

require_once realpath(dirname(__FILE__)) . '/include/common.inc';

/**
 * @param $segment string
 */
function service_account_server ($segment)
{

    if (isset($segment)) {

        $service_accounts = variable_get(VAR_WECHAT_LITE_SERVICE_ACCOUNTS, array());

        if (isset($service_accounts[$segment])) {/*
            drupal_set_title(t('WeChat Service Account'));
            $account = (object)$service_accounts[$segment];

            $form['name'] = array(
                '#type' => 'textfield',
                '#title' => t('Account Name'),
                '#default_value' => $account->name,
            );

            $form['appid'] = array(
                '#type' => 'textfield',
                '#title' => t('Appid'),
                '#default_value' => $account->appid,
            );

            $form['appsecret'] = array(
                '#type' => 'textfield',
                '#title' => t('Appsecret'),
                '#default_value' => $account->appsecret,
            );

            $form['token'] = array(
                '#type' => 'textfield',
                '#title' => t('token'),
                '#default_value' => $account->token,
            );


        }

    }
    else{
        $err_msg = t('Account does not exist.');
        echo $err_msg;
    }


    return $form;*/
            echo $segment;
            return;
        }

        // Only
        //header(t(RET_HTTP_SERVICE_NOT_CONFIG), TRUE, 503);

    }
}


function wechat_lite_callback(){

    $wechat_lite_obj = _wechat_lite_init_obj();
    $wechat_lite_obj->valid();

    $type = $wechat_lite_obj->getRev()->getRevType();
    switch($type){
        case Wechat::MSGTYPE_TEXT: {
            $content = $wechat_lite_obj->getRev()->getRevContent();
            if($content == 'reg'){
                $oauth2 = $wechat_lite_obj->getOauthRedirect('http://xiaoguigh.tunnel.mobi/drupal/?q=wechat_lite_oauth2','1','snsapi_base');
                $text = '<a href="'.$oauth2.'">点击登录</a>';
                $wechat_lite_obj->text($text)->reply();
            }
            else
                $wechat_lite_obj->text('your msgtype is text')->reply();
        }
        case Wechat::MSGTYPE_EVENT: {
            $array = $wechat_lite_obj->getRev()->getRevEvent();
            switch($array['event']){
                case Wechat::EVENT_SUBSCRIBE:
                    $wechat_lite_obj->text('welcome')->reply();
                    break;
                case Wechat::EVENT_UNSUBSCRIBE:
                    break;
            }
            break;
        }
    }

    exit;
}

function oauth2_callback(){

    $wechat_lite_obj = _wechat_lite_init_obj();

    $node = $_GET['state'];
    $userinfo = $wechat_lite_obj->getOauthAccessToken();

    $pwd = '12345678';
    $name = $userinfo['openid'];
    $options = array(
        'name' => $name,
        'mail' => $name.'@geconf.com',
        'pass' => $pwd,
        'status' => 1,
        'init' => 'email address',
        'roles' => array(
            DRUPAL_AUTHENTICATED_RID => 'cyrus_authenticated user',
            4 => 'user',
        ),
    );
    if(!user_load_by_mail($options['mail'])){
        $account = user_save('', $options);
    }

    if($uid = user_authenticate($name, $pwd)){
        $user = user_load($uid);
        $form_state = array();
        $form_state['uid'] = $uid;
        user_login_submit(array(),$form_state);
        drupal_session_regenerate();
        drupal_goto('node/'.$node);
    }


}
