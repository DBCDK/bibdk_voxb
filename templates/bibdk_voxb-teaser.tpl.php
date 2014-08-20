<?php
/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 4/23/14
 * Time: 5:10 PM
 */
?>

<?php foreach($reviews as $review): ?>

<div class="bibdk-voxb-review clearfix">

  <div class="element-section">

    <p>
      <?php print $date_text;?>
      <b><?php print $review->date;?></b>
    </p>

    <p>
      <?php print $author_text;?>
      <b><?php print$review->alias;?></b>
    </p>

  </div>

  <div class="element-section">
    <div class="toggle-next-section">
      <a class="show-more" href="#"><?php print t('view_more'); ?></a>
      <a class="show-less visuallyhidden" href="#"><?php print t('view_less'); ?></a>
    </div>
    <div class="bibdk-edit-review">
      <?php print drupal_render($review->edit_link);?>
    </div>
  </div>

  <div class="element-section visuallyhidden">
    <?php print $review->markup;?>
  </div>

</div>

<?php endforeach;?>
