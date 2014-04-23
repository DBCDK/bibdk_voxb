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
 * @return mixed; stdObject ->ratingCount, ->rating
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
