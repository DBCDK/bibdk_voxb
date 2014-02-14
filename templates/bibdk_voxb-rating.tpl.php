<?php
/**
 * @file
 *
 */
?>
<div class="voxb-details isbn-<?php echo $isbn; ?>">
  <div><?php echo $isbn.' : '.$object->rating.' : '.$object->ratingCount; ?></div>
  <div class="voxb-rating rate-enabled<?php echo $rating_block_class; ?>">
    <?php for ($i = 1; $i <= 5; $i++): ?>
    <?php $star_class = ($i*20 > $object->rating) ? 'star-off' : 'star-on';?>
    <a href="/postgres/voxb/ajax/set_rating/<?php echo $isbn; ?>/<?php echo $i; ?>" class="rating left <?php echo $star_class; ?>" title=" <?php echo $title.' '. $i; ?>"></a>
    <?php endfor; ?>
    <p class="rating-count left"><span></span></p>
  </div>
  <div class="clear"></div>
</div>

