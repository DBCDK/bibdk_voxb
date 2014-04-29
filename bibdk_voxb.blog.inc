<?php
/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 2/27/14
 * Time: 12:23 PM
 */

/**
 * Implements hook_form_FORM_ID_alter (node_blog_form)
 *
 * alter the form here .. e.g add pid in hidden field
 * also add validate and submit methods e.g to save on voxb-webservice
 */
function bibdk_voxb_form_blog_node_form_alter(&$form, &$form_state) {

  if(isset($_GET['pid'])){
  //this is new node or update

  }
  // hide the pid field
  $form['field_pid']['#type'] = 'hidden';

  drupal_set_title(t('voxb_Review'));
  // @TODO set correct title here.
  $form['title']['#title'] = t('TESTHSET', array(), array('context' => 'voxb'));

  $form['field_pid']['#attributes']['readonly'] = TRUE;

  $form['#validate'][] = 'bibdk_voxb_review_validate';

}


function bibdk_voxb_form_search_block_form_alter(&$form,&$form_state){
  dpm(func_get_args());
  //$form['#method']='get';
  $form['#submit'][] = 'no_redirect';
}

function no_redirect(&$form, &$form_state){

  dpm('TESTHSET');
  unset($form_state['redirect']);
}


function bibdk_voxb_review_validate($form, $form_state) {

  dpm(func_get_args());

  $pid = isset($_GET['pid']) ? $_GET['pid'] : NULL;
  if(isset($pid)){
    $form_state['values']['field_pid']['und'][0]['value'] = $pid;
    // or maybe
    $form_state['values']['field_pid'] = $pid;
  }

  // get the node (blog) object
  $blog = (object) $form_state['values'];

  $blog->data['voxb_id'] = 12345;
  node_save($blog);
  if($blog->nid){
    dpm($blog);

}



  if(isset($blog->field_pid)){
    //dpm($blog, 'PID IS SET');
  }
  // get the render array for bocy
  $body = field_view_field('node',$blog,'body');

  dpm($blog);
  dpm($body);
  // extract the text
  $text = $body['#items'][0]['value'];
  dpm($text);

  $voxb_id = bibdk_voxb_user_check();
  $voxb_user = new bibdkVoxbUser($voxb_id);

  $data = $voxb_user->userData();


  // this one assumes that pid is set
  if($row = bibdk_voxb_in_multi_array($pid, $data)){
    // found row is tied to pid. check if review (or rating or ..)
    // @TODO ACTION UPDATE
  }
  else{
  /* @TODO ACTION CREATE ***************


  ******************************/

}


}

/** use this hook for additional access control .. if needed
 * @param $node
 * @param $op
 * @param $account
 */
function bibdk_voxb_node_access($node, $op, $account) {

}



function bibdk_voxb_get_review_form() {


  $faust =  '6755664';

  // check if user is provider
  global $user;
  if( !ding_user_is_provider_user($user)){
    // set an error message for the user
    $error = bibdk_voxb_set_error(t('Please log in to rate', array(), array('context' => 'voxb')), 'status');
    die($error);
  }



  return bibdk_voxb_block_view('bibdk_voxb_blog',$faust);
}

/** Implements hook_block_info
 *
 */
function bibdk_voxb_block_info() {
  $blocks['bibdk_voxb_blog'] = array(
    'info' => t('voxb review as blogenty'),
  );

  return $blocks;
}

/** Implements hook_block_view
 * @return array|mixed
 */
function bibdk_voxb_block_view($delta,$faust) {
  $block = array();
  switch ($delta) {
    case 'bibdk_voxb_blog' :
      $block['content'] = _bibdk_voxb_add_node($faust);
      break;
  }

  return $block;

}

function bibdk_voxb_node_view($node, $view_mode){
  if($node->type == 'blog'){
    dpm($node);
  }
}

/** Wrapper for add_node (@see node.pages.inc). Add node return a themed
 * form. Use this method to insert new reviews as blogs
 *
 * @param $pid
 * @param string $type
 * @return array|mixed
 *
 * @TODO get pid from request
 *
 */
function _bibdk_voxb_add_node($faust = '29857237', $type = 'blog') {
  // @TODO check if review is insert or update
  $voxb_id = bibdk_voxb_user_check();
  if($voxb_id === FALSE){
    // error retrieving or creating user .. do something
    // error has already been watchdogged @see bibdk_voxb.module::bibdk_voxb_user_check
    // set an error for the user
    return FALSE;
  }


  // init a voxb_user
  $voxb_user = new bibdkVoxbUser($voxb_id);
  $data = $voxb_user->userData();

  if(!empty($data)){
    // check if user has already rated this material
    $data_array = bibdk_voxb_in_multi_array($faust, $data);
  }



  //return 'TESTHEST';
  if (!function_exists("node_add")) {
    include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
  }

  $form = node_add($type);



  return $form;
}


