<?php
/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 5/6/14
 * Time: 1:12 PM
 */


/** Wrapper for add_node (@see node.pages.inc). Add node return a themed
 * form for editing a node
 *
 * @param $pid
 * @return array; render array as expected by drupal_render
 *
 */
function bibdk_voxb_review_add_node($type = 'blog') {
  if (!function_exists("node_add")) {
    include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
  }
  // node_add wraps drupal_get_form ($type_node_form).
  $form = node_add($type);
  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter (node_blog_form)
 *
 * alter the form here .. e.g add pid in hidden field
 * also add validate and submit methods e.g to save on voxb-webservice
 */
function bibdk_voxb_form_blog_node_form_alter(&$form, &$form_state) {
  $pid = bibdk_voxb_review_check_pid();
  $user_data = bibdk_voxb_check_voxb_review($pid, $form);
  if(isset($form['#voxb_id'])){
    // @TODO update node, populate form

    if(isset($user_data['voxb:item']['voxb:review'])){
      // populate form with review data

    }
  }
  // hide voxb_id field
 // $form['field_voxb_id']['#type'] = 'hidden';
  // add custom validation method
  $form['#validate'][] = 'bibdk_voxb_review_validate';
}

/** Check if given pid has already been handled by voxb.
 * if so add voxb_identifier to form
 *
 * Get the voxb userid and add it to form also
 *
 * @param $pid
 * @param $form
 */
function bibdk_voxb_check_voxb_review($pid, &$form){
  $faust = bibdk_voxb_pid2faust($pid);
  // add faust to form. we need it in validate function
  $form['#faust'] = $faust;
  $voxb_user_id = bibdk_voxb_user_check();
  // add voxb_user id to form
  $form['#voxb_user_id'] = $voxb_user_id;
  // get data for user
  $data = bibdk_voxb_get_user_item($faust, $voxb_user_id);

  if(empty($data)){
    return;
  }
  // is this a review?
  if ( !(isset($data['voxb:item']['voxb:review'])) ){
    // user is alreade registred, but has no reviews yet
    // @TODO ACT ON IT


  }
  $identifier = $data['voxb:voxbIdentifier']['#text'];
  // add voxb_id to form
  $form['#voxb_id'] = $identifier;
  return $data;
}



/** Custom validator for node_blog_form. Create or updtate voxb-review
 * @param $form
 * @param $form_state
 */
function bibdk_voxb_review_validate(&$form, &$form_state){
  if( !empty($form['#voxb_id'])) {
    $review = bibdk_voxb_voxb_review_from_form($form_state);
    $response = open_voxb_updateReview($form['#voxb_id'], $review);
    try{
      $voxb_id = bibdk_voxb_parse_update_data_response($response);
    }
    catch(bibdkVoxbException $e){

      // malformed xml
      // do something
      //die($e->getMessage());
      form_set_error('submit','HEST');
    }
  }
  else{
    $review = bibdk_voxb_voxb_review_from_form($form_state);
    $user = $form['#voxb_user_id'];

    // create voxb review
    $response = open_voxb_createMyReviewRequest($form['#faust'],$review,$user);
    try{
      $voxb_id = bibdk_voxb_parse_update_data_response($response);
    }
    catch(bibdkVoxbException $e){
      // malformed xml
      // do something

      form_set_error('submit','HEST');
      //die($e->getMessage());
    }

    if ( $voxb_id !== FALSE ){
      $form['#voxb_id'] = $voxb_id;
      $form_state['values']['field_voxb_id']['und'][0]['value'] = $voxb_id;
    }
    else{
      // voxb webservice didn't deliver.
      // do something .. like set error on form
      form_set_error('submit','HEST');
      die('voxb_error');
    }
  }
}

function bibdk_voxb_voxb_review_from_form($form_state){
  $ret->title = $form_state['values']['title'];
  // we can use 'und' when indexing since multilingual suppoert is disabled for blogentries
  // use lang if enabling multilingual support
  $ret->Data = $form_state['values']['body']['und'][0]['value'];

  return $ret;
}




function bibdk_voxb_review_check_pid(){
  static $pid;
  if(!isset($pid)){
    $pid = isset( $_GET['pid'] ) ? $_GET['pid'] : NULL;
  }
  // this is an ajax call json_output and exit
  if( empty($pid)){
    return FALSE;
    $ret = array('error' => bibdk_voxb_set_error(t('No pid in request', array(), array('context' => 'voxb')), 'status'));
    drupal_json_output($ret);
    exit;
  }

  return $pid;
}



function bibdk_voxb_get_review_form() {
  bibdk_voxb_review_check_pid();
  // @TODO this should be ajax. drupal_json_output the return value
  $block = bibdk_voxb_block_view('bibdk_voxb_blog');

  return $block;
}
/********* block definition ***********/

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
function bibdk_voxb_block_view($delta) {
  $block = array();
  switch ($delta) {
    case 'bibdk_voxb_blog' :
      $block['content'] = bibdk_voxb_review_add_node();
      break;
  }
  return $block;
}