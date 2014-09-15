<?php

/** Method to handle direct access to page (not via link on review)
 * @return array
 */
function bibdk_voxb_offensive_error() {
  return array(
    '#markup' => t('bibdk_voxb_no_access', array(), array('context'=>'bibdk_voxb')),
  );
}

/** Get a link to report offensive content
 * @param $voxb_id
 * @return array
 */
function bibdk_voxb_offensive_link($voxb_id) {
  $path = 'voxb/offensive';
  $text = t('bibdk_voxb_offensive_content', array(), array('context' => 'bibdk_voxb'));
  $link = array(
    '#theme' => 'link',
    '#text' => $text,
    '#path' => $path,
    '#options' => array(
      'attributes' => array(),
      'html' => TRUE,
      'query'=>array('voxb_id'=>$voxb_id),
    ),
  );
  return $link;
}

function bibdk_voxb_offensive_form($form, $form_state) {
  $voxb_id = isset ($_GET['voxb_id']) ? $_GET['voxb_id'] : NULL;
  if(empty($voxb_id)){
    return bibdk_voxb_offensive_error();
  }

  if(!ding_user_is_provider_user()){
    $form['login'] = bibdk_voxb_offensive_link_to_login();
    return $form;
  }

  $form['#submit'] = array('bibdk_voxb_offensive_submit');
  $form['#voxb_id'] = array(
    '#type' => 'hidden',
    '#value' => $voxb_id,
  );

  $form['button'] = array(
    '#type' => 'button',
    '#value' => t('bibdk_voxb_offensive_submit', array(), array('context'=>'bibdk_voxb')),
  );

  return $form;
}

/** Get a link to login
 * @return string; rendered array
 */
function bibdk_voxb_offensive_link_to_login() {
  $message = t('bibdk_voxb_login_to_report', array(), array('context' => 'bibdk_voxb'));
  $path = 'user/login';
  $link = array(
    '#theme' => 'link',
    '#text' => $message,
    '#path' => $path,
    '#options' => array(
      'attributes' => array('class' => array('bibdk-voxb-login-to-report')),
      'html' => FALSE,
    ),
  );

  return $link;
}

function bibdk_voxb_offensive_submit ($form, $form_state) {
  // userid and voxb_id are mandatory
  $userId = bibdk_voxb_user_check();
  $voxb_id = $form['#voxb_id'];
  if (empty($userId) || empty($voxb_id)) {
    $message = t('bibdk_voxb_failed_to_report', array(), array('context'=>'bibdk_voxb'));
    form_set_error('submit', $message);
  }


}