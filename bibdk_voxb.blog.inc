<?php
/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 2/27/14
 * Time: 12:23 PM
 */



/*function bibdk_voxb_review_validate($form, $form_state) {
  // get the node (blog) object
  $blog = (object) $form_state['values'];

  // get the render array for bocy
  $body = field_view_field('node',$blog,'body');
  // extract the text
  $text = $body['#items'][0]['value'];

  $voxb_id = bibdk_voxb_user_check();
  $voxb_user = new bibdkVoxbUser($voxb_id);

  $data = $voxb_user->userData();
  dpm($data,'voxb_data:');

  // this one assumes that pid is set
  if($row = bibdk_voxb_in_multi_array($pid, $data)){
    // found row is tied to pid. check if review (or rating or ..)
    // @TODO ACTION UPDATE
  }
  else{
    $pid = $_GET['pid'];
    $faust = bibdk_voxb_pid2faust($pid);
    $review->title = $blog->title;
      $reiew->Data ='HEST';
    dpm($blog);
    $response = open_voxb_createMyReviewRequest($pid, $review, $voxb_id);

  }
  form_set_error('submit','HEST');
}*/

/** use this hook for additional access control .. if needed
 * @param $node
 * @param $op
 * @param $account
 */
function bibdk_voxb_node_access($node, $op, $account) {
  //dpm(func_get_args());
}





