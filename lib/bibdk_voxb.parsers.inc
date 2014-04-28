<?php
/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 2/19/14
 * Time: 12:37 PM
 */

/** Parse response from OUI::bindVoxbRequest
 * @param $xml
 * @return bool|string
 */
function bibdk_voxb_parse_bind_voxb_response($xml) {
  try {
    $xp = bibdk_voxb_get_xpath($xml);
  }
  catch (bibdkVoxbException $e) {
    // rethrow
    throw $e;
  }

  $query = '//oui:userId';
  $nodelist = $xp->query($query);
  if ($nodelist->length != 1) {
    // something is wrong
    return FALSE;
  }

  return $nodelist->item(0)->nodeValue;
}

/** Parse the response from VOXB::createUserRequest
 * @param $xml
 * @return bool|string
 */
function bibdk_voxb_parse_create_user_response($xml) {
  try {
    $xp = bibdk_voxb_get_xpath($xml);
  }
  catch (bibdkVoxbException $e) {
    // rethrow
    throw $e;
  }

  $query = '//voxb:userId';
  $nodelist = $xp->query($query);
  if ($nodelist->length != 1) {
    // something is wrong
    return FALSE;
  }

  return $nodelist->item(0)->nodeValue;
}


/** Parse response from OUI::getVoxb
 * @param $xml
 * @return bool|string; FALSE if something is wrong; voxb_id (userId) from voxb-service
 * @throws Exception
 */
function _bibdk_voxb_parse_get_voxb_response($xml) {
  try {
    $xp = bibdk_voxb_get_xpath($xml);
  }
  catch (bibdkVoxbException $e) {
    // rethrow
    throw $e;
  }

  $query = '//oui:error';
  $nodelist = $xp->query($query);
  if ($nodelist->length > 0) {
    // this is fatal either a database error, or somehow user is not in database
    throw bibdkVoxbException::ouiError();
  }

  $query = '//oui:voxbId';
  $nodelist = $xp->query($query);
  if ($nodelist->length != 1) {
    // this is NOT fatal, but something is wrong
    return FALSE;
  }
  $voxb_id = $nodelist->item(0)->nodeValue;

  return $voxb_id;
}

/** Parse response xml from VOXB::fetchDataRequest
 * @param $xml
 * @return mixed; stdObject ->ratingCount ->rating ->reviews
 * @throws Exception
 */
function bibdk_voxb_parse_fetchdata_request($xml) {
  try {
    $xpath = bibdk_voxb_get_xpath($xml);
  }
  catch (bibdkVoxbException $e) {
    // rethrow
    throw $e;
  }
  // parse for reatings
  $ret = bibdk_voxb_set_ratings($xpath);
  // parse for reviews
  $reviews = bibdk_voxb_set_reviews($xpath);
  if(!empty($reviews)){
    $ret->reviews = $reviews;
  }

  return $ret;
}

/**
 * @param $xpath
 * @return stdObject ->ratingCount, ->rating
 */
function bibdk_voxb_set_ratings($xpath){
  // parse for ratings
  $query = '//voxb:averageRating';
  $nodelist = $xpath->query($query);
  if($nodelist->length > 0){
    $ret->rating = $nodelist->item(0)->nodeValue;
    $query = '//voxb:totalNumberOfRaters';
    $ret->ratingCount = $xpath->query($query)->item(0)->nodeValue;
  }
  if(empty($ret)){
    $ret->ratingCount=(int)0;
    $ret->rating=(int)0;
  }

  return $ret;
}

/** Parse for reivews
 * @param $xpath
 * @return array [stdObject]
 */
function bibdk_voxb_set_reviews($xpath){
  // parse for reviews
  $reviews = array();
  $query = '//voxb:userItems';
  $nodelist = $xpath->query($query);
  if($nodelist->length > 0){
    foreach($nodelist as $node){
      $review = bibdk_voxb_set_review($node);
      if(!empty($review)){
        $reviews[] = $review;
      }
    }
  }
  return $reviews;
}

/** Parse for review. given node is an userItem which may or
 *  may not hold a review
 * @param $node
 * @return stdObject ->alias ->profileLink ->Title ->reviewData ->date
 *
 * @TODO Load review as blog-node ??
 *
 *
 */
function bibdk_voxb_set_review($node){
  $review = $node->getElementsByTagName('review');
  if( $review->length > 0){
    $rev->alias = $node->getElementsByTagName('aliasName')->item(0)->nodeValue;
    $rev->profileLink = $node->getElementsByTagName('profileLink')->item(0)->nodeValue;
    $rev->Title = $node->getElementsByTagName('reviewTitle')->item(0)->nodeValue;
    $rev->reviewData = $node->getElementsByTagName('reviewData')->item(0)->nodeValue;
    $rev->date = $node->getElementsByTagName('timestampModified')->item(0)->nodeValue;

    $identifier = $node->getElementsByTagName('voxbIdentifier')->item(0)->nodeValue;
    $rev->link = bibdk_voxb_review_teaser_link($identifier);
    $rev->teaser = bibdk_voxb_make_review_teaser($rev->reviewData);

    return $rev;
  }
  return FALSE;
}

function bibdk_voxb_review_teaser_link($identifier){
  $path = 'voxb/review/'.$identifier;
  $link = array(
    '#type' => 'link',
    '#title' => t('Read_review',array(), array('context'=>'voxb')),
    '#href' => $path,
    '#attributes'=>array('class'=>array('bibdk-voxb-link')),
  );

  return drupal_render($link);
}

function bibdk_voxb_make_review_teaser($reviewTxt){
  $teaser = substr($reviewTxt,0,100);
  $teaser .= ' ... ';
  return $teaser;
}

/** Initialize an xpath object from given xml
 * @param $xml
 * @return bool|DOMXPath; False on failure, object on success
 */
function bibdk_voxb_get_xpath($xml) {
  $dom = new DOMDocument();
  if (!@$dom->loadXML($xml)) {
    throw bibdkVoxbException::malformedXml($xml);
  }
  $xp = new DOMXPath($dom);
  $xp->registerNamespace('voxb', 'http://oss.dbc.dk/ns/voxb');
  $xp->registerNamespace('oui', 'http://oss.dbc.dk/ns/openuserinfo');

  return $xp;
}

/** Helper function. Check if needle is in multidimensional array
 * @param $needle
 * @param array $haystack
 * @return mixed; the array holding the needle if found, FALSE if not
 */
function bibdk_voxb_in_multi_array($needle, array $haystack) {
  if(empty($haystack)){
    return FALSE;
  }
  foreach ($haystack as $item) {
    // NOTICE recursive
    if (($item == $needle) || (is_array($item) && bibdk_voxb_in_multi_array($needle, $item))) {
      return $item;
    }
  }

  return FALSE;
}
