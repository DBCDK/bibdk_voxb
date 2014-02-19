<?php

/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 2/17/14
 * Time: 1:23 PM
 */
class bibdkVoxbUser
{
    private $userId;
    private $xp;
    private $userData;

    public function __construct($voxb_id) {
        $this->userId = $voxb_id;
    }

    public function getUserId() {
        return $this->userId;
    }

    public function userData() {
        if (!isset($this->userData)) {
            $response = open_voxb_fetchMyDataRequest($this->userId);
            // @TODO errorcheck
            $this->userData=$this->parseFetchMyDataResponse($response);
        }
        return $this->userData;
    }

    private function parseFetchMyDataResponse($xml) {
        $xp = bibdk_voxb_get_xpath($xml);
        if($xp === FALSE){
            return;
        }

        $query = '//voxb:result';
        $nodelist = $xp->query($query);

        if($nodelist->length == 0){
            $this->userData = array();
        }

        foreach( $nodelist as $node ){
            $ret[] = $this->parseNode($node);
        }
        return $ret;
    }

    private function parseNode($node){
        foreach($node->childNodes as $child){
            if($child->hasChildNodes()){
                // NOTICE recursive
               $ret[$child->nodeName] = $this->parseNode($child);
            }
            else{
                $ret[$child->nodeName] = $child->nodeValue;
            }
        }
        return $ret;
    }
}