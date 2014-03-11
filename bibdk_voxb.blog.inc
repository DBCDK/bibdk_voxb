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

  dpm($_GET);

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
  dpm(func_get_args());
}

function bibdk_voxb_review_validate($form, $form_state) {
  //dpm(func_get_args());

  dpm($_GET);

  $pid = isset($_GET['pid']) ? $_GET['pid'] : NULL;
  if(isset($pid)){
    $form_state['values']['field_pid']['und'][0]['value'] = $pid;
  }




  // get the node (blog) object
  $blog = (object) $form_state['values'];
  dpm($blog);

  if(isset($blog->field_pid)){
    dpm($blog, 'PID IS SET');
  }
  // get the render array for bocy
  $body = field_view_field('node',$blog,'body');
  // extract the text
  $text = $body['#items'][0]['value'];

  $voxb_id = bibdk_voxb_user_check();
  $voxb_user = new bibdkVoxbUser($voxb_id);

  $data = $voxb_user->userData();
  dpm($data);

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
  //dpm(func_get_args());
}

function bibdk_voxb_get_review_form() {
  return bibdk_voxb_block_view('bibdk_voxb_blog');
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
function bibdk_voxb_block_view($delta) {
  $block = array();
  switch ($delta) {
    case 'bibdk_voxb_blog' :
      $block['content'] = _bibdk_voxb_add_node();
      break;
  }

  return $block;

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
function _bibdk_voxb_add_node($pid = '870970-basis:29857237', $type = 'blog') {

  // @TODO check if review is insert or update

  //return 'TESTHEST';
  if (!function_exists("node_add")) {
    include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
  }

  $form = node_add($type);

  dpm($form);

  return $form;
}


