<?php
/**
 * Created by IntelliJ IDEA.
 * User: pjo
 * Date: 4/23/14
 * Time: 5:10 PM
 */
?>
<?php foreach($reviews as $review): ?>
<div class="bibdk_voxb_review">
  <?php print $date_text;?>
  <b><?php print $review->date;?></b>
  <br/>
  <?php print $author_text;?>
    <b><?php print$review->alias;?></b>
  <br />

  <div class="element-section">
    <div class="toggle-next-section">
      <a class="show-more" href="#">view_more</a>
      <a class="show-less visuallyhidden" href="#">view_less</a>
    </div>
  </div>
  <div class="element-section visuallyhidden">
    <?php print $review->markup;?>
  </div>

  <span class="icon icon-left icon-darkgrey-infomedia">
  <!--<?php print $review->link;?>-->
  </span>


  <?php endforeach;?>
</div>
<div class="bibdk-article-review clearfix">