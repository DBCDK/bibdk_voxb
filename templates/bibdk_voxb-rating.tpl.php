<?php
/**
 * @file
 *
 */

?>
<div class="voxb-details pid-<?php echo $pid; ?>">

  <div class="voxb-rating rate-enabled">
    <?php for ($i = 1; $i <= 5; $i++): ?>
    <?php $star_class = ($i*20 > $object->rating) ? 'star-off' : 'star-on';?>
    <a href="voxb/ajax/set_rating/<?php echo $pid; ?>/<?php echo $i; ?>" class="rating left <?php echo $star_class; ?>" title=" <?php echo $title.' '. $i; ?>"></a>
    <?php endfor; ?>
    <p class="rating-count left"><span>
        <?php echo t('Number of ratings: ',array(),array('contexb'=>'voxb')) . $object->ratingCount;?>
    </span></p>

  </div>

  <div class="bibdk-article-review">

    <div class="bibdk-write-review-link">
      <?php print drupal_render($review_link);?>
    </div>

  <?php if ( !empty($teasers) ): ?>
    <?php print drupal_render($teasers); ?>
  <?php endif;?>

  </div>

</div>



