<?php
/**
 * @file
 *
 */

?>
<div>
<div class="voxb-details pid-<?php echo $pid; ?>">
  <!--<div><?php echo $object->rating;?></div>-->
  <!--<div><?php echo $object->ratingCount;?></div>-->
  <div class="voxb-rating rate-enabled">
    <?php for ($i = 1; $i <= 5; $i++): ?>
    <?php $star_class = ($i*20 > $object->rating) ? 'star-off' : 'star-on';?>
    <a href="voxb/ajax/set_rating/<?php echo $pid; ?>/<?php echo $i; ?>" class="rating left <?php echo $star_class; ?>" title=" <?php echo $title.' '. $i; ?>"></a>
    <?php endfor; ?>
    <p class="rating-count left"><span>
        <?php echo t('Number of ratings: ',array(),array('contexb'=>'voxb')) . $object->ratingCount;?>
    </span></p>
  </div>
  <div class="bibdk-article-review clearfix"></div>
 <!-- <div class="clear"></div>-->
  <?php print $review_link;?>
  <?php if(!empty($teasers)):?>
    <?php print $teasers;?>
  <?php endif;?>
</div>

</div>

