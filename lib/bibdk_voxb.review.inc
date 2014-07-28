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
  // node_add wraps drupal_get_form ($type_node_form)
  $form = node_add($type);
  drupal_set_title(t('voxb_add_review'),array(),array('context'=>'bibdk_voxb'));
  return $form;
}

function bibdk_voxb_review_edit_node($blog_node){
  drupal_set_title(t('voxb_edit_review'),array(),array('context'=>'bibdk_voxb'));
  if (!function_exists("node_form")) {
    include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
  }
  $form = drupal_get_form('blog_node_form', $blog_node);
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
  if(!empty($pid)){
    $user_data = bibdk_voxb_check_voxb_review($pid, $form);
    $form['#validate'][] = 'bibdk_voxb_review_validate';
  }
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

  $identifier = isset($data['voxb:voxbIdentifier']['#text']) ? $data['voxb:voxbIdentifier']['#text'] : NULL;
  if( !empty($identifier) ) {
    // add voxb_id to form
    $form['#voxb_id'] = $identifier;
  }

  // is this a review?
  /*if ( (isset($data['voxb:item']['voxb:review'])) ){
    // user has already written a review.
    // @TODO ACT ON IT
    $identifier = $data['voxb:voxbIdentifier']['#text'];
    // add voxb_id to form
    $form['#voxb_id'] = $identifier;
  }*/
  $form['#voxb_aliasname'] = $data['alias'];
 // $form['#voxb_alias'] = array('#type'=>'value', '#value' => $data['alias']);

  return $data;
}

function bibdk_voxb_edit_review() {
  $pid = bibdk_voxb_review_check_pid();
  // we need to get the node .. if any
  $form = array();
  $data = bibdk_voxb_check_voxb_review($pid, $form);
  if(isset($form['#voxb_id'])){
    //update
    // try to get the node
    $blog_node = bibdk_voxb_blog_fetch_blog_node($form['#voxb_id']);
    // fetch item from webservice. Content may have changed on voxb-service
    $response = open_voxb_fetchData_by_itemid($form['#voxb_id']);
    $voxb_node = bibdk_voxb_get_review_node($response);
    // synchronize .. item may have change on voxb-service
    $blog_node = bibdk_voxb_review_synchronize($blog_node, $voxb_node);
  }
  else{
    // create
    $blog_node = bibdk_voxb_blog_node_create();
  }



  // @TODO this should be ajax. drupal_json_output the return value
  $block = bibdk_voxb_block_view('bibdk_voxb_blog',$blog_node);

  return $block;
}

/** Synchronize a blog_node with review-node from voxb-webservice
 *
 * the review may have changed on webservice, so we need to update
 * the blog node.
 *
 * @param $blog_node
 * @param $voxb_node
 */
function bibdk_voxb_review_synchronize($blog_node, $voxb_node){
  if(empty($blog_node)){
    $blog_node = bibdk_voxb_blog_node_create($voxb_node);
  }
  // try entity_metadata thingie
 $wrapper = entity_metadata_wrapper('node', $blog_node);
  // alias name may have changed
  $wrapper->field_voxb_alias->set($voxb_node->alias);
  // title may have changed
  if(isset($voxb_node->Title)){
    $wrapper->title->set($voxb_node->Title);
  }
  // body may have change
  if(isset($voxb_node->reviewData)){
    $wrapper->body->set(array('value'=>$voxb_node->reviewData));
  }


  global $user;
  // save the node
  $blog_node->uid = $user->uid;
  return $blog_node;
  //node_save($blog_node);
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
     // die($e->getMessage());
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
  return $pid;
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
function bibdk_voxb_block_view($delta, $blog_node) {
  $block = array();
  switch ($delta) {
    case 'bibdk_voxb_blog' :
    /*  if(empty($blog_node)){
        $block['content'] = bibdk_voxb_review_add_node();
      }
      else{*/
        $block['content'] = bibdk_voxb_review_edit_node($blog_node);
      //}
      break;
  }
  return $block;
}